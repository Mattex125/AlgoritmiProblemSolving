#include <stdio.h>
#include <stdlib.h>
#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define size 10000

void solve(int m,int n,int t){ //solve the problem, max hamburgers in max t
    int result,first,second;
    int dp[size]; //dynamic prog
    dp[0]=0;
    for (int i=1; i<=t;i++) //i means grandezza sottoprob sto risolvendo
    {
        if(i>=m)//ip mangio panino m
            first=dp[i-m]; //ciclo crescente, sarà già calcolato
        else
            first=-1;
        if(i>=n)
            second=dp[i-n];
        else
            second=-1;
        if (first == -1 && second == -1)//non ci sta piu niente, non riesco a riempire
            dp[i]=-1;
        else
            dp[i]=MAX2(first,second)+1;//panini mangiati
    }
    result=dp[t];
    if(result>=0)
        printf("%d\n",result);
    else{   
        int i=t-1;
        for (; result==-1; i--) //negative no, i get before to base ==0
            result=dp[i];

        printf("%d %d\n",result, t-i-1);
    }

}
int main(){
    int m,n,t;
    while(scanf("%d %d %d",&m,&n,&t)==3)
        solve(m,n,t);
//printf("%llu\n", ncallssolve * ncallssolvet);
}