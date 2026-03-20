#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define maxbuffer 1000000
#define A1 40
#define A2 51
#define MOD1 1009
#define MOD2 1007
//list is IA generated, i wanted to be cool and not use an array 
typedef struct listnode {
    int value;
    struct listnode *next;
} listnode;

typedef struct list {
    listnode *head;
    listnode *tail;
} list;

void list_init(list *l) {
    l->head = NULL;
    l->tail = NULL;
}

void list_push_back(list *l, int value) {
    listnode *n = (listnode *)malloc(sizeof(listnode));
    if (n == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->value = value;
    n->next = NULL;

    if (l->tail == NULL) {
        l->head = n;
        l->tail = n;
        return;
    }

    l->tail->next = n;
    l->tail = n;
}

void list_print(const list *l) {
    const listnode *curr = l->head;
    while (curr != NULL) {
        printf("%d", curr->value);
        if (curr->next != NULL) {
            printf(" ");
        }
        curr = curr->next;
    }
    printf("\n");
}
/*
int polinimial_hash(char str[],int A,int B,int len){//not competitive
    int hash=0;
    for(int i=0; i<len; i++){
        hash+=((int)str[i]*(int)pow(A,i))%B;
    }
    return hash;
}*/
void buildHash(char str[], int A,int B, int* powers,int* hashingvalues){
    powers[0]=1;
    //build pows
    for (int i=1; i<strlen(str); i++){
        powers[i]=(powers[i-1]*A)%B; //mod to not overflow
    }
    //build hash
    hashingvalues[0]=powers[0]*str[0]%B;
    for (int i=1; i<strlen(str); i++){ //i got hash until i elem
        //h[i]=h[i]*str+posPOW
        hashingvalues[i] = (hashingvalues[i-1] * A+str[i]) % B;    
    }
    return;
}
int gethash(char str[], int pows[], int hashvals[],int B,int left, int right){
    //hash(l,r) = H[r] - H[l-1] * p^(r-l+1)
    if (left == 0) return hashvals[right];
    return (hashvals[right] - (hashvals[left-1]*pows[right-left+1])%B + B) % B;//+b not -
}
int dosubstrhash(char str[],int B, int hashvals[],int pows[], int i){
    //hash[rip i]=h[i-1]*pow[strlen dx]+h[i+1]
    /*
    sx = hashvals[i-1]*pows[n-i-1]
    dx = (hashvals[n-1] - (hashvals[i]*pows[n-i-1])%B + B) % B;*/
    int sx=0,dx=0;
    int n = strlen(str);
    if (i > 0)
        sx = gethash(str, pows, hashvals, B, 0, i-1);
    if (i < n-1)
        dx = gethash(str, pows, hashvals, B, i+1, n-1);
    return (sx * pows[n-i-1] + dx) % B;
}

int main(){
    char str1[maxbuffer+1],str2[maxbuffer];
    //read input
    scanf("%s %s",str1,str2);
    if (strlen(str1)!=(strlen(str2)+1))
        printf("0");
    else
    {
        int sols=0;
        list* lista=malloc(sizeof(list));
        list_init(lista);
        int j=strlen(str1);
        int j2=strlen(str2);
        //get original hash
        int *p=malloc(sizeof(int)*j2);
        int *h=malloc(sizeof(int)*j2);
        buildHash(str2,A1,MOD1,p,h);
        int realhash1=gethash(str2,p,h,MOD1,0,j2-1);
        buildHash(str2,A2,MOD2,p,h);
        int realhash2=gethash(str2,p,h,MOD2,0,j2-1);
        free(p);
        free(h);
        //now build preproc for real hash
        int *hashvalues1,*powers1,*hashvalues2,*powers2; //i use two hashes for not doing buckets
        powers1=malloc(sizeof(int)*j);
        hashvalues1=malloc(sizeof(int)*j);
        powers2=malloc(sizeof(int)*j);
        hashvalues2=malloc(sizeof(int)*j);
        buildHash(str1,A1,MOD1,powers1,hashvalues1);
        buildHash(str1,A2,MOD2,powers2,hashvalues2);
        for (int i=0; i<j; i++) //per every substr
        {
            int h1=dosubstrhash(str1,MOD1,hashvalues1,powers1,i);
            int h2=dosubstrhash(str1,MOD2,hashvalues2,powers2,i);
            if(h1==realhash1 && h2==realhash2){
                sols++;
                list_push_back(lista,i+1);
            }
        }
        printf("%d\n",sols);
        list_print(lista);
        free(powers1);
        free(powers2);
        free(hashvalues1);
        free(hashvalues2);
    }
//print n of posible sols
//print positions to remove
return 0;
}


    