#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define mrows 3001
#define mcols 3001
//not test...
// complessita': O(r*c*h*w*log(r*c)) - per ogni rettangolo faccio binary search su k e ogni check costa O(h*w)

typedef int board[mrows][mcols];

int compare(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int median(int topr,int lcol,int brow, int rcol, board q){ //passo rett
    int i,j,num_cur_ret = 0;
    static int cur_rectangle[mrows*mcols];//placeholder x sort
    
    // Scorri il rettangolo e popola cur_rectangle
    for(i = topr; i <= brow; i++) {
        for(j = lcol; j <= rcol; j++) {
            cur_rectangle[num_cur_ret] = q[i][j];
            num_cur_ret++;
        }
    }
    
    // Sorta gli elementi
    qsort(cur_rectangle, num_cur_ret, sizeof(int), compare);
    
    // Ritorna la mediana
    return cur_rectangle[num_cur_ret / 2]; //mi va bene parte intera bcs è dispari
}

// Verifica se un rettangolo ha mediana <= k (cioe' almeno need valori <= k)
int feasible_median_leq(int topr, int lcol, int brow, int rcol, board q, int k){
    int i,j;
    int cnt = 0;
    int n = (brow - topr + 1) * (rcol - lcol + 1);
    int need = n/2 + 1;
    for(i = topr; i <= brow; ++i){
        for(j = lcol; j <= rcol; ++j){
            if(q[i][j] <= k) {
                if(++cnt >= need) return 1;
            }
        }
    }
    return 0;
}

int rectangle(int r, int c, int h, int w, board q){
    int trow,lcol,brow,rcol;
    int best = INT_MAX;

    for(trow = 0; trow + h <= r; ++trow){
        for(lcol = 0; lcol + w <= c; ++lcol){//per ogni quadrattino pos, i do binsearch for best mediana
            brow = trow + h - 1;
            rcol = lcol + w - 1;

            // se non posso migliorare best, skippo il rettangolo
            if(best != INT_MAX && !feasible_median_leq(trow, lcol, brow, rcol, q, best - 1))
                continue;//go next iteration

            // cerca il minimo k tale che mediana <= k
            int low = 1;
            int high = (best == INT_MAX) ? (r * c) : (best - 1);
            int res = high;
            while(low <= high){
                int mid = (low + high) >> 1;
                if(feasible_median_leq(trow, lcol, brow, rcol, q, mid)){
                    res = mid;
                    high = mid - 1;
                } else low = mid + 1;
            }

            if(res < best) 
                best = res;
        }
    }

    return best;
}
