// http://poj.org/problem?id=3258

#include <stdio.h>
#include <stdlib.h>

int canI_J(unsigned char *rocks, int j, int m, int L) {
	int last_seen_rock = 0;

	for (int whereiam = 1; whereiam <= L; ++whereiam) {
		if (rocks[whereiam]) {//se vedo roccia
			if (whereiam - last_seen_rock < j) {//if whereiam -last rock < jump --> RM

				if (whereiam == L) {//gotta rm the penultimate rock, i cannto delete last
					//rocks[last_seen_rock] = 0;
					--m;
				} else {
					/* This rock makes the jump too small, so we delete it. */
					//rocks[whereiam] = 0;
					--m;
				}
				if (m < 0) 
				    return 0;
			} else //jump ok, now i remember this new rock
				last_seen_rock = whereiam;
		}
	}

	return 1;
}

void solve(unsigned char *rocks, int L, int m){
    int low=0;
    int high=L+1;
    while (high-low>1)
    {
		int mid = low + (high - low) / 2;
        if(canI_J(rocks,mid,m,L))
            low=mid;
        else
            high=mid;
    }
    printf("%d\n",low); //why not hight
}

int main(void) {
	int L, N, M;
	scanf("%d %d %d", &L, &N, &M);

	unsigned char *rocks = calloc((size_t)L + 1, sizeof(unsigned char));
	rocks[0] = 1;
	rocks[L] = 1;
	for (int i = 0; i < N; ++i) {
		int position;
        scanf("%d", &position);
		if (position >= 0 && position <= L) {//sanity pos check
			rocks[position] = 1;
		}
	}
    solve(rocks,L,M);
	free(rocks);
	return 0;
}

//memory exxeded, cambialo per non usare bitmap