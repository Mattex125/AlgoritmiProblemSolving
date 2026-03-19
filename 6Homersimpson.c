#include <stdio.h>
#include <stdlib.h>
#define MAX2(a,b) ((a) > (b) ? (a) : (b))
static unsigned long long int ncallssolvet = 0;
static unsigned long long int ncallssolve = 0;

int solve_t(int m,int n, int t){//cerca di riempire in T
    ncallssolvet++;
    if(t==0)
        return 0;
    int first;
    if (t>=m) 
        first=solve_t(m,n,t-m);
    else
        first=-1;
    int second;
        if (t>=n)
        second=solve_t(m,n,t-n);
    else
        second=-1;
    if (first == -1 && second == -1)//non ci sta piu niente, non riesco a riempire
        return -1;
    return MAX2(first,second)+1;//max panini per riempimento
}

void solve(int m,int n,int t){ //solve the problem, max hamburgers in max t
    ncallssolve++;
    int result=solve_t(m,n,t);
    if(result>=0)
        printf("%d\n",result);
    else{
        int i=0;
        for (i=t-1; i>=0 && result==-1; i--){
            result=solve_t(m,n,i);
        }
        if(result==-1)
            printf("0\n");
        else
            printf("%d %d\n",result,i);
    }
}

int main(){
    int m,n,t;
    scanf("%d %d %d",&m,&n,&t);
    solve(m,n,t);
//printf("%llu\n", ncallssolve * ncallssolvet);
}