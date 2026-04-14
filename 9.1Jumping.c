#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX 

int min2(int a, int b) {
    return (a < b) ? a : b;
}

int solve(int fee[],int rncost,int whereiam,int n,int lastjump,int **memo){
    if (memo[whereiam][lastjump]!=-1) //info da cui posso dedurreof?  futuro
        return memo[whereiam][lastjump];
    //base case i got to end
    if(whereiam==n-1)
        return rncost;
    //i am out of bounds
    if(whereiam>n-1 || whereiam<0)
        return INF;
    //recursion
    int next=whereiam+lastjump+1;
    int sol1=INF; //forward
    int sol2=INF; //backward
    if(next<n)
        sol1=solve(fee, rncost+fee[next],next, n, lastjump+1,memo); //same that prof
    if(whereiam-lastjump>=0)
        sol2=solve(fee,rncost+fee[whereiam-lastjump],whereiam-lastjump,n,lastjump,memo);

    return memo[whereiam][lastjump]=min2(sol1,sol2);
}


int main(void) {
    int n;
    //read N 
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    //allock and read fee
    int *fee = malloc(n * sizeof(int));
    if (fee == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &fee[i]);
    }
    if (n < 2) {
        printf("0\n");
        free(fee);
        return 0;
    }

    int **memo=malloc(sizeof(int*)*n);
    for (int i=0;i<n;i++){
        int *x=malloc(sizeof(int)*n);
        memo[i]=x;
        for(int j=0;j<n;j++){
            memo[i][j]=-1;
        }
    }
    // Nikola starts at square 1 (index 0)
    // first jump must be to square 2 (index 1), length = 1
    int ans = solve(fee, fee[1], 1, n, 1,memo);
    printf("%d\n", ans);    
    
    for(int i=0; i<n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", memo[i][j]);
        }
        printf("\n");
    }
    free(fee);
    return 0;
}
/*
8
4
17
16
13
2
5
18
10

64
*/