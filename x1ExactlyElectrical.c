//https://dmoj.ca/problem/ccc17j3

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x, y;
} Point;

bool can_connect_exactly(Point a, Point b, int n) {
    int d = abs(a.x - b.x) + abs(a.y - b.y);  // axis-only distance
    return (n >= d) && ((n - d) % 2 == 0); //return true
    //if it's reachable and since wasting takes 2 steps we can take optimal (axist real distance +2n)
}

int main(void) {
    Point start, end;
    int n;
    // input: x1 y1 x2 y2 n
    scanf("%d %d %d %d %d", &start.x, &start.y, &end.x, &end.y, &n);
    printf("%s\n", can_connect_exactly(start, end, n) ? "Y" : "N");
    return 0;
}
