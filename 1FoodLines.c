#include <stdio.h>

int shortest_line_index(int lines[], int n) {
  int j;
  int shortest = 0; //looking for minor as selection sort
  for (j = 1; j < n; j++)
    if (lines[j] < lines[shortest])
      shortest = j;
  return shortest;
}

void solve(int lines[], int n, int m) {
  int i, shortest;
  for (i = 0; i < m; i++) {  //for every new person we put it on shortest line
    shortest = shortest_line_index(lines, n);
    printf("%d\n", lines[shortest]);
    lines[shortest]++;
  }
}

#define len 100
int main(){
    int n, m; // n - number of lines, m - number of people
    scanf("%d%d",&n,&m);
    int lines[len]; //lines[i] - number of people in line i
    for (int i=0; i<n; i++) {
        scanf("%d",&lines[i]);
    }
    solve(lines, n, m);
    return 0;
}
