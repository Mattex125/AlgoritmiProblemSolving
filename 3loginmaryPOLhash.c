//https://dmoj.ca/problem/coci17c1p3hard

//gioca con le costanti
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 100000
#define MAX_PSW 10

#define NUM_BITS 20

typedef struct psw_node{
    char password[MAX_PSW+1];
    int total;
    struct psw_node* next;
}psw_node;

unsigned int polynomial_hash(const char *key, unsigned bits) {
    const unsigned int a = 911382323u;
    const unsigned int b = 1000000007u;
    unsigned int hash = 0;

    while (*key) {
        hash = (unsigned int)(((unsigned long long)hash * a + (unsigned char)(*key)) % b);
        key++;
    }

    return hash & ((1u << bits) - 1u);
}

psw_node *inhashtable(psw_node *hash_table[],char *find){ //cerca find in the hash table
    unsigned code;
    psw_node *password_ptr;
    code=polynomial_hash(find, NUM_BITS);
    password_ptr=hash_table[code];// scorro la lista
    while(password_ptr!=NULL)
    {
        if(strcmp(password_ptr->password,find)==0)
            return password_ptr; //password found
        password_ptr=password_ptr->next;
    }
    return NULL;
}

void add2hashtable(psw_node *hash_table[], char *find){
    unsigned code;
    psw_node *passoword_ptr;
    passoword_ptr=inhashtable(hash_table,find);
    if (!passoword_ptr)
    {
        code=polynomial_hash(find, NUM_BITS);
        passoword_ptr=(psw_node *)malloc(sizeof(psw_node));
        strcpy(passoword_ptr->password,find);
        passoword_ptr->total=0;
        passoword_ptr->next=hash_table[code];
        hash_table[code]=passoword_ptr;
    }
    passoword_ptr->total++;
}

//a volte fa errori capire why
int main() {
    static psw_node* hashtable[1<<NUM_BITS]={NULL};//2 alla 20 slots
    //leggo q, n ops da fare
    int num_op,op;
    char psw[MAX_PSW+1],substr[MAX_PSW+1]; //placeholders
    psw_node* passwordptr; //new passnode that i'll be added
    scanf("%d",&num_op);//read n ops
    while (num_op>0) //while ops
    {
        scanf("%d",&op); //che op eseguo
        scanf("%10s",psw);//password
        
        switch (op) 
        {
        case 1: //add
            // a string of length n has exactly n*(n+1)/2 distinct positional substrings (sum first n numbers)
            char *seen[(MAX_PSW*(MAX_PSW+1))/2]={NULL};
            int seen_count=0; //how many subs strings seen

            int n=strlen(psw);

            for (int i=0; i<n;i++)
                for(int j=i; j<n;j++)
                {
                    int alreadyinhash=0;

                    strncpy(substr,&psw[i],j-i+1); //creo substr
                    substr[j-i+1]='\0';

                    // scan only the substrings we already stored, not the whole array
                    for (int h=0;h<seen_count;h++)
                    {
                        if(seen[h]!=NULL)
                        {
                            if(strcmp(seen[h],substr)==0)//duplicated
                                alreadyinhash=1;
                        }
                    }

                    if(alreadyinhash==0)
                    {
                        //we allocate new arr for newsubstr
                        seen[seen_count]=(char*)malloc(strlen(substr)+1);
                        strcpy(seen[seen_count],substr);
                        seen_count++;

                        add2hashtable(hashtable,substr);
                    }
                }

            // release all allocated for this password's substrings
            for(int h=0;h<seen_count;h++) { free(seen[h]); seen[h]=NULL; }

            break;

        case 2: //query
            passwordptr=inhashtable(hashtable,psw);
            if(!passwordptr)
                printf("0\n");
            else
                printf("%d\n",passwordptr->total);
            break;
        }

        num_op--;
    }

    return 0;
}