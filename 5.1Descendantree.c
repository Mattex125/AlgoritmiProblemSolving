#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 10
#define DIMHashTable 1000000
#define MAXTREE 1000 
//as we can see via result, the work for maintaining a hash table is too expensive for n elems we have
int hash_oaat(char *str) {
    int hash = 0;
    while (*str) {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        str++;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

typedef struct node{
    int nchildren;
    struct node **children; /*ptr to an array of ptrs*/
    int score;
    char *name;
}node;

typedef struct bucket{
    node *n;
    struct bucket *next;
}bucket;

typedef struct cacheNames{ //cache names to know if already there or not
    bucket* hashtable[DIMHashTable];
}cacheNames;

void *mallocsafe(size_t size){
    void *mem = malloc(size);
    if (mem == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return mem;
}

node *findnode(node** nodes, int num_nodes, char *name, cacheNames *hashtable){//cerco nel maxiarrays, se trovo il nome do il suo ptr
    int hashnome=abs((int)(hash_oaat(name) % DIMHashTable)); //use abs bcs with int i sometimes had overflow 
    if (hashtable->hashtable[hashnome] != NULL) //if there is on hashtablet
    { //look for bucket
        bucket *b=hashtable->hashtable[hashnome];
        while (b!=NULL)
        {
            if (strcmp(b->n->name,name)==0)
                return b->n;
            else
                b=b->next;
        }
    }
    return NULL;
}

node *newnode(char *name,cacheNames *hashT){//creo un nodo con il suo nome
    node *n=(node *)mallocsafe(sizeof(node));
    n->name=name;
    n->nchildren=0;
    n->children=NULL;//intanto che non ho figli setto a null
    //insert in hash
    int hashcode=abs((int)(hash_oaat(name) % DIMHashTable));
    bucket* placeholder;
    bucket* newnode=(bucket *)mallocsafe(sizeof(bucket));
    //create new bucket elem
    newnode->n=n;
    //new list head
    placeholder=hashT->hashtable[hashcode];
    hashT->hashtable[hashcode]=newnode;
    newnode->next=placeholder;
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

int readtrees(node ** nodes,int nlines, cacheNames *hashtable){ //quanti nodi ho aggiunto
    node *parent,*child;
    char *pname,*cname;
    int numchilden;
    int nnodes=0;
    for(int i=0; i<nlines;i++){
        pname=(char *)mallocsafe(MAXNAME+1);//salvo nome 
        //read the line until n children
        scanf("%s",pname);
        scanf("%d",&numchilden);
        parent=findnode(nodes,nnodes,pname,hashtable); //cerco nel array se ho gia allocato costui
        if (parent==NULL){
            parent=newnode(pname,hashtable);
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
            child=findnode(nodes,nnodes,cname,hashtable);
            if(child==NULL)//if there is not child
                {
                    child=newnode(cname,hashtable);
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
    if (n <= 0) return;
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

void freehashtable(cacheNames *hashtable){
    for (int i = 0; i < DIMHashTable; i++){
        bucket *curr = hashtable->hashtable[i];
        while (curr != NULL){
            bucket *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(hashtable);
}

int main()
{
    int num_tests;
    scanf("%d",&num_tests);
    for (int i=0; i<num_tests;i++){ //per every test
        cacheNames *tabellahash = (cacheNames *)calloc(1, sizeof(cacheNames));
        int num_lines,d;
        scanf("%d %d",&num_lines,&d); //starting
        node** nodes=(node **)mallocsafe(sizeof(node*)*MAXTREE);
        int totalread=readtrees(nodes,num_lines,tabellahash);
        scoreall(nodes,totalread,d);
        //
        printf("Tree %d:\n",i+1);
        getwinners(nodes,totalread);
        printf("\n");
        freenodes(nodes,totalread);
        freehashtable(tabellahash);
        }
    return 0;
}