//FORWARD LOGIC

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 1000
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define INF INT_MAX 

//+1 solo per contare da 1
int solve_ij(int cost[], int n, int i, int j, int memo[n + 1][n + 1]) {
    int first, second;
    if (memo[i][j] != -2)
        return memo[i][j];
//base case
    if (i == n) { //i'm on destination
        return memo[i][j] = 0;
    }
    if (i + j + 1 <= n) // i can go forward
        first = solve_ij(cost, n, i + j + 1, j + 1, memo);
    else
        first = -1;

    if (i - j >= 1) // i can go backward
        second = solve_ij(cost, n, i - j, j, memo);
    else
        second = -1;

    if (first==-1 && second==-1) //no sol
        return memo[i][j]=-1;
    if (second==-1)
        return memo[i][j]=first+cost[i+j+1]; //opzione 1
    if (first==-1)
        return memo[i][j]=second+cost[i-j]; //opzione 2
    return memo[i][j]=MIN(first+cost[i+j+1],second+cost[i-j]);
}

int solve(int cost[], int n){
    int i, j;
    int result;
    int best;
    int (*memo)[n + 1] = malloc((n + 1) * sizeof(*memo));
    if (memo == NULL) {
        return -1;
    }

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++)
            memo[i][j] = -2;
    }

    if (n == 1) {
        free(memo);
        return 0;
    }

    best = INF;
    result = solve_ij(cost, n, 2, 1, memo);
    if (result != -1) {
        best = MIN(best, cost[2] + result);
    }

    free(memo);
    if (best == INF) {
        return -1;
    }
    return best;
}

int main(){
    int n;
    static int cost[SIZE + 1];

    if (scanf("%d", &n) != 1) {
        return 1;
    }

    for (int i = 1; i <= n; i++) {
        if (scanf("%d", &cost[i]) != 1) {
            return 1;
        }
    }

    printf("%d\n", solve(cost, n));
    return 0;
}