// http://poj.org/problem?id=3258

#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b) {
	int ia = *(const int *)a;
	int ib = *(const int *)b;
	return (ia < ib) ? -1 : (ia > ib);
}

/* Greedy check over sorted positions: positions[0..count-1].
   Return 1 if it's possible to have minimal jump >= j by removing at most m rocks. */
int canI_J(const int *positions, int count, int j, int m) {
	int last_kept = 0; /* index in positions of last kept rock */
	int removed = 0;

	for (int i = 1; i < count; ++i) {
		int dist = positions[i] - positions[last_kept];
		if (dist < j) {
			++removed; /* we would remove positions[i] */
			if (removed > m) return 0;
		} else {
			last_kept = i; /* keep this rock */
		}
	}

	return 1;
}

int main(void) {
	int L, N, M;
	if (scanf("%d %d %d", &L, &N, &M) != 3) return 0;

	int count = N + 2;
	int *positions = malloc((size_t)count * sizeof(int));
	if (positions == NULL) return 1;

	positions[0] = 0;
	positions[count - 1] = L;
	for (int i = 1; i <= N; ++i) {
		if (scanf("%d", &positions[i]) != 1) {
			free(positions);
			return 1;
		}
	}

	qsort(positions, (size_t)count, sizeof(int), compare_ints);

	int low = 0;
	int high = L + 1;
	while (high - low > 1) {
		int mid = low + (high - low) / 2;
		if (canI_J(positions, count, mid, M))
			low = mid;
		else
			high = mid;
	}

	printf("%d\n", low);
	free(positions);
	return 0;
}
