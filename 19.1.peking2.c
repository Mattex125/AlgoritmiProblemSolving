#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 50000
#define LABEL_LENGTH 16


typedef struct segtree_node
{
    int left,right;
    int maxidx;
}segtree_node;

typedef struct treap_node {
  char * label;
  int priority;
} treap_node;

// Ritorna il massimo tra due interi
static inline int max_int(int a, int b){
    return (a > b) ? a : b;
}

void init_segtree(segtree_node segtree[], int node, int left, int right){
    int mid;
    //metto apposto la radice
    segtree[node].left=left;
    segtree[node].right=right;
    //base case
    if(left==right)//segment len 0 (es: 4-4)
        return;
    //sec case
    mid=(left+right)/2;
    //heap array prop: root-->*2 --> child
    init_segtree(segtree, node*2, left, mid);//parte sx
    init_segtree(segtree, node*2+1, mid, right);//parte dx
}

//returns idx elem max from below
int fill_segtree(segtree_node segtree[], int node, treap_node treap_nodes[]){
    //create max of segs
    //like heap, we use best child
    int leftmax, rightmax;
    //base case
    if(segtree[node].left==segtree[node].right) //we're on leaf
        return segtree[node].maxidx;
    //rec case
    leftmax=fill_segtree(segtree, node*2, treap_nodes);//lookup leaft tree
    rightmax=fill_segtree(segtree, node*2+1, treap_nodes);
    
    if(treap_nodes[leftmax].priority > treap_nodes[rightmax].priority)//bigger sx
        return segtree[node].maxidx=rightmax; //return whats best idx
    else
        return segtree[node].maxidx=leftmax;
     
}

int compare(const void *v1, const void *v2) {
  const treap_node *n1 = v1;
  const treap_node *n2 = v2;
  return strcmp(n1->label, n2->label);
}

int main(void) {
  static treap_node treap_nodes[MAX_NODES];
  static segtree_node segtree[MAX_NODES * 4 + 1];
  int num_nodes, i;
  scanf("%d ", &num_nodes);
  while(num_nodes>0){
    for (i = 0; i < num_nodes; i++) {
      treap_nodes[i].label = read_label(LABEL_LENGTH);
      scanf("%d ", &treap_nodes[i].priority);
    }
    qsort(treap_nodes, num_nodes, sizeof(treap_node), compare);
    init_segtree(segtree, 1, 0, num_nodes - 1);
    fill_segtree(segtree, 1, treap_nodes);
    solve(treap_nodes, 0, num_nodes - 1, segtree);
    printf("\n");
    scanf("%d ", &num_nodes);
  }
  return 0;

}