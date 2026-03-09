//https://dmoj.ca/problem/coci17c1p3hard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 100000
#define MAX_PSW 10

int main() {
    static char users[MAX_USER][MAX_PSW+1];
    int num_op,op;
    char password[MAX_PSW+1];
    int nusers=0;
    //leggo q, n ops da fare
    scanf("%d",&num_op);
    while (num_op>0)
    {
        scanf("%d",&op); //che op eseguo
        switch (op) 
        {
        case 1: //add
            scanf("%10s", password);
            strcpy(users[nusers],password);
            nusers++;
            break;
        
        case 2: //query
        {
            int total=0;
            scanf("%10s", password);
            for (int j=0; j<nusers;j++)
                if (strstr(users[j],password))
                    total++;
            printf("%d\n",total);
            break;
        }
        }
        num_op--;
    }

    return 0;
}