#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxbuffer 1000000
#define BITS 20

typedef struct node
{
    struct node *next;
    int pos;
}node;

void remove_char(char *s, int pos) {//IA generated
    while (s[pos] != '\0') {
        s[pos] = s[pos + 1];
        pos++;
    }
}

int comparatoresenzaI(char s1[],char s2[],int i){
    char copy[maxbuffer+1];
    strcpy(copy, s1);   // copia la stringa
    remove_char(copy, i); // rimuovi carattere k
    return strcmp(copy,s2);
}

int main(){
    char str1[maxbuffer+1],str2[maxbuffer];
    node* head=NULL;
    //read input
    if (scanf("%1000000s %1000000s",str1,str2) != 2) {
        return -1;
    }
        //printf("str1: %s\n", str1);
        //printf("str2: %s\n", str2);
    if (strlen(str1)!=strlen(str2)+1)
        printf("0");
    else{
    int sols=0;
    int i=0;
    int j=(int)strlen(str2);
    while (i<=j)
    {
        int s=comparatoresenzaI(str1,str2,i);
        if (s==0)
        { //adding list of sols
            node* newposto=(node *)malloc(sizeof(node));
            newposto->pos=i+1;
            newposto->next=head;
            head=newposto;
            sols++;
        }
        i++;
    }
    printf("%d\n",sols);
    while (head!=NULL)
    {
        printf("%d ",head->pos);
        node *deallocator=head;
        head=head->next;
        free(deallocator);
    }
    }
//print n of posible sols
//print positions to remove
}
