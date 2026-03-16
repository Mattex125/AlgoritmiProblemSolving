#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// simple macro function for maximum between two values
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct tree_node
{
    int candy;
    struct tree_node* l;
    struct tree_node* r;
}tree_node,root;
//we'll not use a parent node

int tree_candys(root * t){
    if (!t->l&&!t->r){ //both are null
        return t->candy;
    }
    return tree_candys(t->l)+tree_candys(t->r);
}

int nleaves(root* t){
    if (!t->r && !t->l)
        return 1;
    return nleaves(t->l)+nleaves(t->r);
}

int nnodes(root* t){
    if (!t->r && !t->l)
        return 1;
    return nnodes(t->l)+nnodes(t->r)+1;
}

int nstreets(root* t){
    if (!t->r && !t->l)//i cannot go down elsewhere
        return 0;
    return nstreets(t->l)+nstreets(t->r)+4;//others + go r/t and get back r/l 
}

int htree(root* t){
    if (!t->r && !t->l)//i cannot go down elsewhere
        return 0;
    return MAX(htree(t->l),htree(t->r))+1;
}

void sol(root* t){
    int candy=tree_candys(t);
    int height=htree(t);
    int numstreets=nstreets(t)-height;
    printf("%d %d\n",numstreets,candy);
}

tree_node* readtree(char in[],int * idx){
    root* currentNode=(root*) malloc(sizeof(root));

    //while(in[*idx] == ' ') //skip whitespaces
    //    (*idx)++;

    if (in[*idx]=='('){
        (*idx)++;
        currentNode->l=readtree(in,idx);
        //while(in[*idx]==' '){
        (*idx)++;
        //}
        currentNode->r=readtree(in,idx);
        //while(in[*idx]==' '){
        //(*idx)++;
        //}
        (*idx)++;   // skip ')'
    }
    if (in[*idx] >= '0' && in[*idx] <= '9'){ //fucking reading 2or more digits
        int value = 0;
        while(in[*idx] >= '0' && in[*idx] <= '9'){
            value = value * 10 + (in[*idx] - '0');
            (*idx)++;
        }
        currentNode->candy = value;
        currentNode->l = NULL;
        currentNode->r = NULL;
        (*idx)++;
    }
    return(currentNode);
}
void deallocate_tree(root* t){
    if(t->l==NULL && t->l==NULL)
    free(t);
    return
    deallocate_tree(t->l);
    deallocate_tree(t->r);
}

int main(){
    char str[256+1];
    for(int k=0;k<5;k++){//input says 5 test x time
        int i=0;
        scanf(" %[^\n]", str);
        //fgets(str, 257, stdin);
        root *albero=readtree(str,&i);
        sol(albero);
    }
}