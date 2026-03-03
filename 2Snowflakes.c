#include <stdio.h>
#include <stdlib.h>

// TO DO: hashing map of the sum of elems for do less work and using static arrays
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

void identify_identical(int **snowflakes, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (areID(snowflakes[i], snowflakes[j])) {
                printf("Twin snowflakes found.\n");
                return;
            }
        }
    }
    printf("No two snowflakes are alike.\n");
}

int main(void) {
    int nele, i, j;
    scanf("%d", &nele);
    
    int **snowflakes = (int **)malloc(nele * sizeof(int *));
    for (i = 0; i < nele; i++)
        snowflakes[i] = (int *)malloc(6 * sizeof(int));
    
    for (i = 0; i < nele; i++)
        for (j = 0; j < 6; j++)
            scanf("%d", &snowflakes[i][j]);
    
    identify_identical(snowflakes, nele);
    
    for (i = 0; i < nele; i++)
        free(snowflakes[i]);
    free(snowflakes);
    
    return 0;
}