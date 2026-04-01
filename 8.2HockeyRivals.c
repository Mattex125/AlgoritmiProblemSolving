//Dy 2 dims
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000

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

//why pass dp like this
int solve(char outcome1[],int goals1[],char outcome2[],int goals2[],int n){ //i and j means lenght of  considered games x 1 or 2
    int option1,option2,option3,option4; //options from notes
    static int dp[SIZE+1][SIZE+1];
    for (int k=0; k<=n;k++)
        dp[0][k]=0;
    for (int k=0; k<=n;k++)
        dp[k][0]=0;
    for(int i=1; i<SIZE+1; i++){
        for(int j=1; j<SIZE+1; j++){
            if ((outcome1[i]=='W' && outcome2[j]=='L' && goals1[i]>goals2[j]) || (outcome1[i]=='L' && outcome2[j]=='W' && goals1[i]<goals2[j])){//option 1
                option1=dp[i-1][j-1]+goals1[i]+goals2[j];
            }
            else
            option1=0;
            option2=dp[i-1][j-1];
            option3=dp[i-1][j];
            option4=dp[i][j-1];

            //
            int scelta;
            dp[i][j]=maxOf4(option1,option2,option3,option4,&scelta);
        }
    }
    return dp[n][n];//per n partite di tutti e due
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
    result = solve(team1, goals1, team2, goals2, n);
    printf("%d\n", result);
    return 0;
}