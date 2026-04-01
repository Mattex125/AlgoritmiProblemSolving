#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 28

int maxOf4(int option1, int option2, int option3, int option4, int *choice) {
    if (option1 >= option2 && option1 >= option3 && option1 >= option4) {
        *choice = 1;
        return option1;
    }
    if (option2 >= option3 && option2 >= option4) {
        *choice = 2;
        return option2;
    }
    if (option3 >= option4) {
        *choice = 3;
        return option3;
    }
    *choice = 4;
    return option4;
}

int solve(char outcome1[],int goals1[],char outcome2[],int goals2[],int i, int j){ //i and j means lenght of  considered games x 1 or 2
    int option1,option2,option3,option4; //options from notes
    if(i==0 || j==0) //no games from 1 team, no rivals
        return 0;
    //maybe partita rivale, in entrambi i sensi
    if ((outcome1[i]=='W' && outcome2[j]=='L' && goals1[i]>goals2[j]) || (outcome1[i]=='L' && outcome2[j]=='W' && goals1[i]<goals2[j])){//option 1
        option1=solve(outcome1,goals1,outcome2,goals2,i-1,j-1)+goals1[i]+goals2[j];//why this rec works?
    }
    else
        option1=0;
    //option 2
    option2=solve(outcome1,goals1,outcome2,goals2,i-1,j-1);
    //option 3/4
    option3=solve(outcome1,goals1,outcome2,goals2,i-1,j);
    option4=solve(outcome1,goals1,outcome2,goals2,i,j-1);
    int choice=0;
    return maxOf4(option1,option2,option3,option4,&choice);
}
int main(){
    int i,n,result;
    char team1[SIZE+1],team2[SIZE+1];
    int goals1[SIZE+1],goals2[SIZE+1];
    scanf("%d",&n);
    for (i = 1; i <= n; i++) {
        scanf(" %c", &team1[i]);
    }
    for (i = 1; i <= n; i++) {
        scanf("%d", &goals1[i]);
    }
    for (i = 1; i <= n; i++) {
        scanf(" %c", &team2[i]);
    }
    for (i = 1; i <= n; i++) {
        scanf("%d", &goals2[i]);
    }
    result = solve(team1, goals1, team2, goals2, n, n);
    printf("%d\n", result);
    return 0;
}