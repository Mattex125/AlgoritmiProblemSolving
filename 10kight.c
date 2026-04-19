#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct position {
  int row, col;
} position;

typedef int board[MAX_ROWS + 1][MAX_COLS + 1];

typedef position positions[MAX_ROWS * MAX_COLS];

void add_position(int from_row, int from_col,
                  int to_row, int to_col, int num_rows, int num_cols,
                  positions new_positions, int *num_new_positions,
                  board min_moves) {
    struct position new_position;

    if (to_row >= 1 && to_col >= 1 && to_row <= num_rows && //if legal and not visited i visit
        to_col <= num_cols && min_moves[to_row][to_col] == -1) {
        min_moves[to_row][to_col] = 1 + min_moves[from_row][from_col];
        new_position = (position){to_row, to_col};
        new_positions[*num_new_positions] = new_position;
        (*num_new_positions)++;
    }
}

int find_distance(int knight_row, int knight_col, //knight gets every space 
                  int dest_row, int dest_col,
                  int num_rows, int num_cols) {
    positions cur_positions, new_positions;
    int num_cur_positions, num_new_positions;
    int i, j, from_row, from_col;
    board min_moves;

    for (i = 1; i <= num_rows; i++) //memo of min moves
        for (j = 1; j <= num_cols; j++)
            min_moves[i][j] = -1;

    //set starts
    min_moves[knight_row][knight_col] = 0;
    cur_positions[0] = (position){knight_row, knight_col};
    num_cur_positions = 1;

    while (num_cur_positions > 0) { //while can explore
        num_new_positions = 0;

        for (i = 0; i < num_cur_positions; i++) {
            from_row = cur_positions[i].row;
            from_col = cur_positions[i].col;

            if (from_row == dest_row && from_col == dest_col)//already seen
                return min_moves[dest_row][dest_col];
            //else i explore
            add_position(from_row, from_col, from_row + 1, from_col + 2,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row + 1, from_col - 2,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row - 1, from_col + 2,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row - 1, from_col - 2,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row + 2, from_col + 1,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row + 2, from_col - 1,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row - 2, from_col + 1,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
            add_position(from_row, from_col, from_row - 2, from_col - 1,
                         num_rows, num_cols, new_positions,
                         &num_new_positions, min_moves);
        }

        num_cur_positions = num_new_positions; //n new pos
        for (i = 0; i < num_cur_positions; i++) //re set arrray
            cur_positions[i] = new_positions[i];
    }
    //nothing to explore
    return -1;
}

int main() {
    int ngames, nr, nc, pr, pc, kr, kc;
    scanf("%d",&ngames);
    while (ngames>0)
    //t is used to mesaure the changing of pos pawn
    {
        scanf("%d %d %d %d %d %d",&nr,&nc,&pr,&pc,&kr,&kc);
        int esito=-1,minplay,esito_moves=nr-pr;
        for (int pawmoves=pr+1;pawmoves<nr && esito==-1;pawmoves++){ //i try to win before promotion
            int t = pawmoves - pr; //turni passed
            minplay=find_distance(kr,kc,pawmoves,pc,nr,nc);
            if (minplay!=-1 && minplay<=t && (t - minplay) % 2 == 0) { //i won, esito vincente, in tempo e syncronized
                esito=1;
                esito_moves=t; //time passed
            }
        }
        if (esito==-1) //if i dont win
        {
            int cur_pawn_row = pr; //current pos i'll be move
            int t = 0;

            // qui simulo il pedone passo passo
            // e controllo la casella *davanti* al pedone
            while (cur_pawn_row < nr && esito == -1) {
                int target_row = cur_pawn_row + 1;

                int minplay = find_distance(kr, kc, target_row, pc, nr, nc);

                //can i stalemate?
                if (minplay != -1 && minplay <= t && (t - minplay) % 2 == 0) {
                    esito = 2;
                    esito_moves = t;
                }

                cur_pawn_row++;
                t++;
            }

        }
        switch (esito) {
            case 1:
                printf("Win in %d knight move(s).\n", esito_moves);
                break;
            case 2:
                printf("Stalemate in %d knight move(s).\n", esito_moves);
                break;
            default:
                printf("Loss in %d knight move(s).\n", nr - pr-1);// last turn is already won
                break;
        }
        ngames--;
    }
    return 0;
}