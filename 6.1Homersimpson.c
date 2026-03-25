#include <stdio.h>
#include <stdlib.h>
#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define size 1000

int solve_t(int m,int n, int t,int memo[]){//cerca di riempire in T
    if(memo[t]!=-2) //look for cache
        return memo[t];
    if(t==0)
        return memo[t]=0;//assegno e returno l'assegno
    int first; //se mangio primo panino
    if (t>=m) 
        memo[t-m]=first=solve_t(m,n,t-m,memo);//RIDONDANTE! LO FACCIO GIA ALLA FINE
    else
        first=-1;
    int second;//se mangio secondo panino
        if (t>=n)
        second=solve_t(m,n,t-n,memo);
    else
        second=-1;
    if (first == -1 && second == -1)//non ci sta piu niente, non riesco a riempire
        return memo[t]=-1;
    return memo[t]=MAX2(first,second)+1;//max panini per riempimento+questo
}

void solve(int m,int n,int t){ //solve the problem, max hamburgers in max t
    int result;
    int memo[size];
    for (int i=0; i<=t;i++)
        memo[i]=-2; //deciso da noi come unknow
    result=solve_t(m,n,t,memo);
    if(result>=0) //se ho trovato result preciso
        printf("%d\n",result);
    else{ //non ho sol precisa, birra
        int i=0;
        for (i=t-1; i>=0 && result==-1; i--){
            result=solve_t(m,n,i,memo);
        }
        if(result==-1)
            printf("0\n");
        else
            printf("%d %d\n",result,t - i - 1);//advanced time
    }
}

int main(){
    int m,n,t;
    while(scanf("%d %d %d",&m,&n,&t)==3)
        solve(m,n,t);
//printf("%llu\n", ncallssolve * ncallssolvet);
}