#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX-1

/*
BREAKS STACK, REC 0-->5, 5-->0 ...
*/

int scegli_min_no_negativi(int n1, int n2, int *best) {
    if (n1 < 0 && n2 < 0) {
        *best = -1;
        return 0;
    }

    if (n1 < 0) {
        *best = n2;
        return 2;
    }

    if (n2 < 0) {
        *best = n1;
        return 1;
    }

    if (n1 <= n2) {
        *best = n1;
        return 1;
    }

    *best = n2;
    return 2;
}

int tryjump_rec(int j, int h, int nj[], int pos) { //jump, h2reach, not jump, actualpos
    int max_pos = h + j;
    if(pos<0)
        return INF;//can do pruning
    if (pos>=h) //base case! i won
        return 0; 
    int option1,option2;
    if (pos + j > max_pos || nj[pos+j]==1) //check bounds before reading nj[pos+j]
        option1=INF;
    else
        option1=1+tryjump_rec(j, h, nj, pos + j);//rec case-->1+what i'll come
    //why bfs and not recursione (for every distance i try jump behind)
    option2=INF;
    int option2pos=pos;
    for (int i=pos-1 ;i>=0 && i>=pos-j;i--){//i try all jumps behind, never below 0
        if(i<=max_pos && nj[i]!=1){
            int attemp=1+tryjump_rec(j, h, nj, i);//i start from pos-1,pos-2....
            if(attemp<option2){
                option2=attemp;
                option2pos=i;
            }
        }
    }
    //when i finish i choose best possible min
    int best;
    int scelta_best=scegli_min_no_negativi(option1,option2,&best);
    return best;
}

int solve(int j, int h, int nj[]) {
    return tryjump_rec(j, h, nj, 0);
}

int main(){
    // nj[x] = 1 means Bob cannot hold the rope at height x.
    int H, J, N;
    if (scanf("%d %d %d", &H, &J, &N) != 3) {
        return 0;
    }

    int max_h = H + J;
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