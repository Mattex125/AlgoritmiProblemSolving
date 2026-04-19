#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX-1


int tryjump_rec(int j, int h, int nj[], int pos) { //jump, h2reach, not jump, actualpos
    //we will to a bfs to all pos until 2h so we choice the best
    int *neededmoves=malloc((h+1)*sizeof(int));
    neededmoves[0]=0;
    for (int i=1; i<h+1; i++)
        neededmoves[i]=-1;
    int *current=malloc((h+1)*sizeof(int));
    int *new=malloc((h+1)*sizeof(int));
    current[0]=pos; //add node 0 to queue
    int newdiscovered=1;
    while (newdiscovered>0){
        int currentnew=0;
        for(int i=0;i<newdiscovered;i++){ //i explore
            int x=current[i];
            if(x>=h/2) //base case
                {
                int result=neededmoves[x];
                free(current);
                free(new);
                free(neededmoves);
                return result;
                }
            // i go forward
            int nextjumpland=x+j;
            if(nextjumpland >= 0 && nextjumpland <= h && nj[nextjumpland] != 1 && neededmoves[nextjumpland] == -1) {
                new[currentnew++]=nextjumpland;
                neededmoves[nextjumpland]=neededmoves[x]+1;
            }
            //i go backwards
            for (int k=x; k>0; k--){
                if(nj[k]!=1 && neededmoves[k]==-1){
                    new[currentnew++]=k;
                    neededmoves[k]=neededmoves[x]+1;
                }
            }
        }
        //prepare for next arch relaxaxion
        int *tmp=current;
        current=new;
        new=tmp;
        newdiscovered=currentnew;
    }
    free(current);
    free(new);
    free(neededmoves);
    return -1;
}

int solve(int j, int h, int nj[] ) {
    return tryjump_rec(j, 2*h, nj, 0);
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