#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 50000
#define LABEL_LENGTH 16

typedef struct treap_node {
  char * label;
  int priority;
} treap_node;

int max_priority_index(treap_node treap_nodes[], int left, int right) {
  int i;
  int max_index = left;
  for (i = left + 1; i <= right; i++)
    if (treap_nodes[i].priority > treap_nodes[max_index].priority)
      max_index = i;
  return max_index;
}

void solve(treap_node treap_nodes[], int left, int right) {
  int root_index;
  treap_node root;
  if (left > right)
    return;
  root_index = max_priority_index(treap_nodes, left, right);
  root = treap_nodes[root_index];
  printf("(");
  solve(treap_nodes, left, root_index - 1);
  printf("%s/%d", root.label, root.priority);
  solve(treap_nodes, root_index + 1, right);
  printf(")");
}

char *read_label(int size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (str == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != '/')) {
    str[len++] = ch;
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (str == NULL) {
        fprintf(stderr, "realloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

int compare(const void *v1, const void *v2) {
  const treap_node *n1 = v1;
  const treap_node *n2 = v2;
  return strcmp(n1->label, n2->label);
}

int main(void) {
  static treap_node treap_nodes[MAX_NODES];
  int num_nodes, i;
  scanf("%d ", &num_nodes);
  while (num_nodes > 0) {
    for (i = 0; i < num_nodes; i++) {
      treap_nodes[i].label = read_label(LABEL_LENGTH);
      scanf("%d ", &treap_nodes[i].priority);
    }
    qsort(treap_nodes, num_nodes, sizeof(treap_node), compare);
    solve(treap_nodes, 0, num_nodes - 1);
    printf("\n");
    scanf("%d ", &num_nodes);
  }
  return 0;
}
