#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 10

typedef struct node
{
    int nchildren;
    struct node **children; /*ptr to an array of ptrs*/
    int score;
    char *name; //why name[] error?
}node;

void *mallocsafe(size_t size){
    void *mem = malloc(size);
    if (mem == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return mem;
}

node *findnode(node** nodes, int num_nodes, char *name){//cerco nel maxiarrays, se trovo il nome do il suo ptr
    for(int i=0;i<num_nodes;i++)
    {
        if(strcmp(nodes[i]->name,name)==0) //found
            return nodes[i];//return ptr i knwo were is that node
    }
    return NULL; //not in array
}

node *newnode(char *name){//creo un nodo con il suo nome
    node *n=(node *)mallocsafe(sizeof(node));
    n->name=name;
    n->nchildren=0;
    n->children=NULL;//intanto che non ho figli setto a null
    return n;
}


int score_one(node* node,int d){
    //non cotrllo se è vuoto perche non uso albero, uso array, se non ha figli ha numfigli=0
    if(d==1) //base case
        return node->nchildren;
    int score=0;
    for(int i=0;i<node->nchildren;i++){//recursion
        score+=score_one(node->children[i],d-1);
    }
    return score;
}


//array di tutti i nodi che ho, questo perche l'albero non è conesso fin da subito

void scoreall(node ** nodes, int nnodes, int d){
    for (int i=0;i<nnodes;i++)
        nodes[i]->score=score_one(nodes[i],d);
}

int readtrees(node ** nodes,int nlines){ //quanti nodi ho aggiunto
    node *parent,*child;
    char *pname,*cname;
    int numchilden;
    int nnodes=0;
    for(int i=0; i<nlines;i++){
        pname=(char *)mallocsafe(MAXNAME+1);//salvo nome 
        //read the line until n children
        scanf("%s",pname);
        scanf("%d",&numchilden);
        parent=findnode(nodes,nnodes,pname); //cerco nel array se ho gia allocato costui
        if (parent==NULL){
            parent=newnode(pname);
            nodes[nnodes++]=parent;//aggiungo maxiarray
        }
        else //padre c'era gia
        {
            free(pname);
        }
        //now we gotta read children
        parent->children=(node **)mallocsafe(sizeof(node*)*numchilden);//alloco array children
        parent->nchildren=numchilden;
        //per every children
        for(int j=0;j<parent->nchildren;j++){
            cname=(char *)mallocsafe(MAXNAME+1);
            scanf("%s",cname);
            child=findnode(nodes,nnodes,cname);
            if(child==NULL)//if there is not child
                {
                    child=newnode(cname);
                    nodes[nnodes++]=child;
                }
            else
            {
                free(cname);
            }
            //entriamo nel nodo padre e lo colleghiamo ai children
            parent->children[j]=child;
        }
    }
    return nnodes;
}

int cmp2nodes(const void *a,const void *b){
    //standard for sorting
    node *n1 = *(node **)a;//ptr to a pointer(array) that points to a ptr node
    node *n2 = *(node **)b;

    if(n1->score>n2->score)
        return -1;
    if(n1->score<n2->score)
        return 1;
    return strcmp(n1->name, n2->name);
}

void getwinners(node ** nodes, int n){
    qsort(nodes,n,sizeof(node *),cmp2nodes);
    //print podium
    int i;
    for(i=0; i<3 && nodes[i]->score>0; i++)
        printf("%s %d\n",nodes[i]->name,nodes[i]->score);
    while (nodes[2]->score==nodes[i]->score && nodes[i]->score>0){//if tie print until break
        printf("%s %d\n",nodes[i]->name,nodes[i]->score);
        i++;}   
}

void freenodes(node ** nodes,int n){
    for(int i=0;i<n;i++){
        free(nodes[i]->children);
        free(nodes[i]->name);
        free(nodes[i]);
    }
    free(nodes);
}
int main()
{   //my erros was to not understand input, num lines!=num nodes
    int num_tests;
    scanf("%d",&num_tests);
    for (int i=0; i<num_tests;i++){ //per every test
        int num_lines,d;
        scanf("%d %d",&num_lines,&d); //starting
        node** nodes=(node **)mallocsafe(sizeof(node*)*1000000);
        int totalread=readtrees(nodes,num_lines);
        scoreall(nodes,totalread,d);
        //
        printf("Tree %d:\n",i+1);
        getwinners(nodes,totalread);
        printf("\n");
        freenodes(nodes,totalread);
        }
    return 0;
}

//fare tree candy non ricorsiva
//in questo problema, usare hash
//trovare avi (ascendenti) in dist k (0 se non esiste
//) prima in o(n) poi in log(n)
//es 4 da fare a csaa