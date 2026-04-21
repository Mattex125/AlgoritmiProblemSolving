#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
   Adjacency-list node:
   one directed edge from current list owner -> to, with traversal time.
*/
typedef struct Edge {
	int to;
	int time;
	struct Edge *next;
} Edge;

/*
   Graph container (1-based indexing):
   adj[u] points to the linked list of outgoing edges from node u.
*/
typedef struct {
	int n;          /* number of nodes (1..n) */
	Edge **adj;     /* adjacency lists */
} Graph;

/* Forward declaration because reverse_graph may free a graph on failure. */
static void free_graph(Graph *g);

/* Allocate a graph with n nodes and empty adjacency lists. */
static Graph *create_graph(int n) {
	Graph *g = (Graph *)malloc(sizeof(Graph));
	if (g == NULL) {
		return NULL;
	}

	g->n = n;
	g->adj = (Edge **)calloc((size_t)(n + 1), sizeof(Edge *));
	if (g->adj == NULL) {
		free(g);
		return NULL;
	}

	return g;
}

/* Insert one directed edge: from -> to with given time cost. */
static int add_edge(Graph *g, int from, int to, int time) {
	Edge *new_edge;

	if (g == NULL || from < 1 || from > g->n || to < 1 || to > g->n) {
		return 0;
	}

	new_edge = (Edge *)malloc(sizeof(Edge));
	if (new_edge == NULL) {
		return 0;
	}

	new_edge->to = to;
	new_edge->time = time;
	new_edge->next = g->adj[from];
	g->adj[from] = new_edge;

	return 1;
}

/*
   Build a new graph with all edges reversed.
   If original has u -> v (w), reversed has v -> u (w).
*/
static Graph *reverse_graph(const Graph *g) {
	int from;
	Graph *rev;

	if (g == NULL) {
		return NULL;
	}

	rev = create_graph(g->n);
	if (rev == NULL) {
		return NULL;
	}

	for (from = 1; from <= g->n; ++from) {
		Edge *curr = g->adj[from];
		while (curr != NULL) {
			/* Reverse each directed edge: from -> to becomes to -> from. */
			if (!add_edge(rev, curr->to, from, curr->time)) {
				free_graph(rev);
				return NULL;
			}
			curr = curr->next;
		}
	}

	return rev;
}

/*
   Dijkstra from start on (already reversed) graph.
   Returns how many nodes have shortest distance <= max_time.
*/
static int dijkstra_count_reachable(const Graph *g, int start, int max_time) {
	int i;
	int n;
	int *dist;
	int *used;
	int count = 0;

	if (g == NULL || start < 1 || start > g->n) {
		return 0;
	}

	/* Allocate arrays used by classic O(N^2) Dijkstra. */
	n = g->n;
	dist = (int *)malloc((size_t)(n + 1) * sizeof(int));
	used = (int *)calloc((size_t)(n + 1), sizeof(int));
	if (dist == NULL || used == NULL) {
		free(dist);
		free(used);
		return 0;
	}

	/* Initialize all nodes as unreachable, except the source node. */
	for (i = 1; i <= n; ++i) {
		dist[i] = INT_MAX;
	}
	dist[start] = 0;

	/* O(N^2) Dijkstra: good enough for this maze-size context. */
	for (i = 1; i <= n; ++i) {
		int u = -1;
		int v;

		/* Pick the unvisited node with smallest tentative distance. */
		for (v = 1; v <= n; ++v) {
			if (!used[v] && dist[v] != INT_MAX && (u == -1 || dist[v] < dist[u])) {
				u = v;
			}
		}

		if (u == -1) {
			/* No more reachable unvisited nodes. */
			break;
		}

		/* No need to continue once shortest available path already exceeds T. */
		if (dist[u] > max_time) {
			break;
		}

		used[u] = 1;

		/* Relax outgoing edges from the selected node. */
		{
			Edge *edge = g->adj[u];
			while (edge != NULL) {
				/* Relaxation with overflow guard before addition. */
				if (dist[u] <= INT_MAX - edge->time && dist[u] + edge->time < dist[edge->to]) {
					dist[edge->to] = dist[u] + edge->time;
				}
				edge = edge->next;
			}
		}
	}

	/* Count nodes that can reach the exit within the allowed time. */
	for (i = 1; i <= n; ++i) {
		if (dist[i] <= max_time) {
			count++;
		}
	}

	free(dist);
	free(used);
	return count;
}

/* Release all edge nodes, then adjacency array, then graph struct. */
static void free_graph(Graph *g) {
	int i;

	if (g == NULL) {
		return;
	}

	for (i = 1; i <= g->n; ++i) {
		Edge *curr = g->adj[i];
		while (curr != NULL) {
			Edge *next = curr->next;
			free(curr);
			curr = next;
		}
	}

	free(g->adj);
	free(g);
}

/*
   Input format:
   - number of test cases
   - for each case: N, E, T, M then M directed edges (a b t)
   Output:
   - one line per test case: number of cells that can reach exit E within time T
*/
int main(void) {
	int tcases;
	int current_case = 0;

	/* Read total number of test cases. */
	if (scanf("%d", &tcases) != 1 || tcases <= 0) {
		return 0;
	}

	/* Process each test case independently. */
	while (current_case < tcases) {
		int j;
		int N, E, T, M;
		int reachable_count;
		Graph *g;
		Graph *reversed;

		/* Read maze metadata for current test case. */
		if (scanf("%d %d %d %d", &N, &E, &T, &M) != 4) {
			fprintf(stderr, "Invalid input while reading test case %d.\n", current_case + 1);
			return 1;
		}

		/* Create original directed graph. */
		g = create_graph(N);
		if (g == NULL) {
			fprintf(stderr, "Memory allocation failed for graph of test case %d.\n", current_case + 1);
			return 1;
		}

		/* Read and insert the M directed edges. */
		for (j = 0; j < M; ++j) {
			int a, b, t;
			if (scanf("%d %d %d", &a, &b, &t) != 3) {
				fprintf(stderr, "Invalid edge input in test case %d.\n", current_case + 1);
				free_graph(g);
				return 1;
			}

			if (!add_edge(g, a, b, t)) {
				fprintf(stderr, "Failed to add edge (%d -> %d) in test case %d.\n", a, b, current_case + 1);
				free_graph(g);
				return 1;
			}
		}

		/* Invert graph so one Dijkstra from E solves all-to-exit shortest paths. */
		reversed = reverse_graph(g);
		if (reversed == NULL) {
			fprintf(stderr, "Failed to invert graph in test case %d.\n", current_case + 1);
			free_graph(g);
			return 1;
		}

		/* Replace original graph with its reversed version. */
		free_graph(g);
		g = reversed;

		/* Run Dijkstra from exit E on reversed graph and count cells within T. */
		reachable_count = dijkstra_count_reachable(g, E, T);

		/* Print result exactly as required by common online judge format. */
		printf("%d\n", reachable_count);
		if (current_case < tcases - 1) {
			/* Blank line between consecutive test-case outputs. */
			printf("\n");
		}

		/* Clean up memory for this test case before moving to the next one. */
		free_graph(g);
		current_case++;
	}

	return 0;
}
