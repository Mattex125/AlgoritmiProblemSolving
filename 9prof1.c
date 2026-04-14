#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX 
#define SIZE 100000

//BACKWARD REASONING

int min2(int a, int b) {
    return (a < b) ? a : b;
}

//i square arrivo, j is jump
int solve_ij(int fee[],int n,int i,int j,int memo[][n+1]){//solve subprob for i and j
    int op1,op2;
    if (memo[i][j]!=-2)
        return memo[i][j];
    //base case
    if (i==2 && j==1){
        return memo[i][j]=0;
    }
    //salto a dx
    if(i-j>=1 && j>=2) //casella valida and casella di almeno 1
        op1=solve_ij(fee,n,i-j,j-1,memo);
    else
        op1=-1;
    //salto a sx
    if (i+j<=n)//casella valida e non fuori
        op2=solve_ij(fee,n,i+j,j,memo);
    else
        op2=-1;
    if(op1==-1 && op2==-1) //subprob imp
        return memo[i][j]=-1; //li non è possibile risolvere
    else{
        if(op2==-1)
            return memo[i][j]=op1+fee[i];
        if(op1==-1)
            return memo[i][j]=op2+fee[i];
        //se sono both valid
        return memo[i][j]=min2(op1,op2)+fee[i];
    }
}
int solve(int cost[], int n){
    int i,j,best,result;
    int (*memo)[n+1] = malloc((n + 1) * sizeof(*memo));
    if (memo == NULL) {
        return -1;
    }
    for(i=0;i<=n;i++){
        for(j=0; j<=n;j++)
            memo[i][j]=-2;
    }
    best=INF;
    for(j=1;j<=n;j++){
        result=solve_ij(cost,n,n,j,memo);
        if(result!=-1){
            best=min2(best,cost[2]+result);
        }
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