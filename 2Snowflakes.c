#include <stdio.h>
#include <stdlib.h>

//RICORDA: typedef struct in alto senno dopo non sembrano utilizzabili
typedef struct Snowflake_node
{
    int snowflake[6];
    struct Snowflake_node *next;
}Snowflake_node;

int IDright(int *s1,int *s2, int start){
    for (int offset=0;offset<6;offset++){
        if (s1[offset]!=s2[(start+offset)%6]){
            return 0;
        }
    }
    return 1;
}

int IDleft(int *s1,int *s2, int start){
    for (int offset=0;offset<6;offset++){
        if (s1[offset]!=s2[(start-offset+6)%6]){
            return 0;
        }
    }
    return 1;
}

int areID(int *s1, int *s2){
    for (int start=0; start<6; start++){
        if (IDright(s1,s2,start)!=0)
            return 1;
        if (IDleft(s1,s2,start)!=0)
            return 1;
    }
    return 0;
}

#define SIZE 100000 //scelto in base a tradeoff di collissions
//big size meno collisioni ma array piu grande
//meno troppe colliisoni diventa array

void identify_identical(Snowflake_node *snowflakes[], int n) {// i want to not compare all snowflakes
//fisso un nodo e lo scorro con quelli dopo until i found null 

    Snowflake_node *node1,*node2;
    for (int i=0;i<n;i++)//Sscorro TUTTA la lista
    {
        node1=snowflakes[i];
        while(node1!=NULL) //fisso node 1
        {
            node2=node1->next; //per ogni nodo 2
            while(node2!=NULL)
            {
                if(areID(node1->snowflake,node2->snowflake))
                {
                    printf("Twin snowflakes found.\n");
                    return;
                }
                node2=node2->next;
            }//sposto ogni nodo, confronti n*(n-1)
        node1=node1->next;
        }
    }
    //se ho scorso la lista e nulla
    printf("No two snowflakes are alike.\n");
}


int code(int snowflake[])//input array di 6 numeri
{
    int sum=0;
    for (int i=0; i<6;i++)
    {
        sum=sum+snowflake[i];
    }
    sum=sum%SIZE; //HASING VIA DIVISIONE
    return sum;
}



int main(void) {
    int nele, i, j,snowflake_code;
    scanf("%d", &nele);
    static Snowflake_node *snowflakes[SIZE]={NULL}; //array liste 
    Snowflake_node *snow;
    for (i=0;i<nele;i++)
    {
        //placeholder
        snow=(Snowflake_node *)malloc(sizeof(Snowflake_node));
        //metto snowflake nel placeholder
        for (j=0; j<6;j++)
        {
            scanf("%d",&snow->snowflake[j]);
        }
        //ora gestisco fila, ptr snow su array e quello che c'era su next ?
        //lo metto per primo, e metto ptr del attuale infondo
        snowflake_code=code(snow->snowflake);
        snow->next=snowflakes[snowflake_code];
        snowflakes[snowflake_code]=snow;
    }
    identify_identical(snowflakes,SIZE);
    return 0;
}