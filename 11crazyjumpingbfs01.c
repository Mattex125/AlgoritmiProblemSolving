#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX-1

//DEVI CONTOLLARE CHE SIA MINORE PERCHE ANCHE SE VADO INDIETRO E METTO IN CURRENT MAGARI FACCIO UN AVANTI E FACCIO +1

typedef struct {
    int pos;
    int rope; /* 0 rope su, 1 = rope giu */
} State;

int tryjump_rec(int j, int h, int nj[], int pos, int dist[][2]) { //jump, h2reach, not jump, actualpos
    /*
     * 0-1 BFS su stati (pos, rope) con due fronti current/new.
     * Pesi richiesti:
     * - andare su (rope=0): +1
     * - andare giu (rope=1): +0
     * - cambio 0->1: +1
     * - cambio 1->0: +0
     */
    int max_pos = 2 * h - 1; /* search space cap as in reference (from H to 2H-1) */
    int state_count = (max_pos + 1) * 2; // max number of (pos, rope) states in a frontier
    State *current = malloc((size_t)state_count * sizeof(State));
    State *new = malloc((size_t)state_count * sizeof(State));

    for (int i = 0; i <= max_pos; i++) {
        dist[i][0] = -1;
        dist[i][1] = -1;
    }

    /* metto 0 in coda */
    dist[pos][0] = 0;
    current[0] = (State){pos, 0};
    int current_count = 1;

    while (current_count > 0) { //relax edges
        int new_count = 0;

        for (int i = 0; i < current_count; i++) {
            State cur = current[i];
            int cur_dist = dist[cur.pos][cur.rope];

            /* Base case: primo stato estratto che raggiunge H in rope=0 è garantito minimo. */
            if (cur.pos >= h && cur.rope == 0) {
                free(current);
                free(new);
                return cur_dist;
            }

            /* Caso 1: andare su (rope=0), costo +1 -> prossimo fronte. */
            if (cur.rope == 0) {
                int next_pos = cur.pos + j;
                int nd = cur_dist + 1;
                if (next_pos >= 0 && next_pos <= max_pos && nj[next_pos] != 1 &&
                    (dist[next_pos][0] == -1 || dist[next_pos][0] > nd)) {
                    dist[next_pos][0] = nd;
                    new[new_count++] = (State){next_pos, 0};
                }
            }

            /*
             * Caso 2: andare giu (rope=1), costo +0 -> stesso fronte.
             *
             * Ottimizzazione importante:
             * prima scandivi fino a j posizioni indietro (O(j) per stato),
             * qui facciamo solo un passo (pos-1) in O(1).
             * Questo riduce molto il tempo totale sui casi grandi.
             */
            if (cur.rope == 1) {
                int k = cur.pos - 1;
                int nd = cur_dist;
                if (k >= 0 && (dist[k][1] == -1 || dist[k][1] > nd)) {
                    dist[k][1] = nd;
                    current[current_count++] = (State){k, 1}; //SAME TIME, GET IN CURRENT NOT NEXT
                }
            }

            /* Caso 3: cambio 0->1, costo +1 -> prossimo fronte. */
            if (cur.rope == 0) {
                int nd = cur_dist + 1;
                if (dist[cur.pos][1] == -1 || dist[cur.pos][1] > nd) {
                    dist[cur.pos][1] = nd;
                    new[new_count++] = (State){cur.pos, 1};
                }
            }

            /* Caso 4: cambio 1->0, costo +0 -> stesso fronte. */
            if (cur.rope == 1) {
                int nd = cur_dist;
                if (nj[cur.pos] != 1 && (dist[cur.pos][0] == -1 || dist[cur.pos][0] > nd)) {
                    dist[cur.pos][0] = nd;
                    current[current_count++] = (State){cur.pos, 0}; //SAME TIME, GET IN CURRENT NOT NEXT
                }
            }
        }

        /* Passo al fronte successivo (distanza +1). */
        {
            State *tmp = current;
            current = new;
            new = tmp;
            current_count = new_count;
        }
    }

    free(current);
    free(new);
    return -1;
}

int solve(int j, int h, int nj[] ) {
    int max_pos = 2 * h;
    int (*dist)[2] = malloc((size_t)(max_pos + 1) * sizeof(*dist));
    if (dist == NULL) {
        return -1;
    }

    int ans = tryjump_rec(j, h, nj, 0, dist);
    free(dist);
    return ans;
}

int main(){
    // nj[x] = 1 means Bob cannot hold the rope at height x.
    int H, J, N;
    if (scanf("%d %d %d", &H, &J, &N) != 3) {
        return 0;
    }

    int max_h = 2 * H;
    int *nj = (int *)calloc((size_t)(max_h + 1), sizeof(int));
    if (nj == NULL) {
        return 0;
    }

    for (int i = 0; i < N; i++) {
        int A, B;
        if (scanf("%d %d", &A, &B) != 2) {
            free(nj);
            return 0;
        }

        if (B < 0 || A > max_h) {
            continue;
        }
        if (A < 0) {
            A = 0;
        }
        if (B > max_h) {
            B = max_h;
        }

        for (int x = A; x <= B; x++) {
            nj[x] = 1;
        }
    }

    int ans = solve(J, H, nj);
    printf("%d\n", ans);

    free(nj);
    return 0;
}
