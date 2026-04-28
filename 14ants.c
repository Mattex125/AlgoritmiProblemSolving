//leggere albero come lista di adiacenza

#include <stdio.h>
#include <stdlib.h>
#define MAX_NODES 1000
#define H 2e9

//DIVISIONE NETTA FRA ALGO CHE DICE SE è FEASABLE E BIN SEARCH SULLA FEASABLE

typedef struct edge {
	int to;
	int perc;
	int superpipe;
	struct edge *next;
} edge;


void add_edge(edge **adjlist, int from, int to, int perc, int superpipe){
	edge *e = (edge *)malloc(sizeof(edge));
	e->to = to;
	e->perc = perc;
	e->superpipe = superpipe;
	e->next = adjlist[from];
	adjlist[from] = e;
}

//FEASABILITY
int can_feed(int node, double liquid, edge*adj[],int liquidneed[]){//rec sol
    //base case --> FOGLIA
    if (liquidneed[node]!=-1)
        return liquid>=liquidneed[node];//bool controll
    //rec case --> 
    else{
        edge* e=adj[node];
        int ok=1;
        while(e!=NULL && ok){//while ho da scorrete e tutto ok
            double liquiddown=liquid*e->perc/100.0;
            if(e->superpipe==0)
                ok=can_feed(e->to,liquiddown,adj,liquidneed);
            else
                ok=can_feed(e->to,liquiddown*liquiddown,adj,liquidneed);
            e=e->next;
        }
        return ok;
    }
}

//BIN SEARCH
void solve(edge *adj[], int liquidn[]){
    double low=0, high=H, mid;
    //if mid good --> go for low
    while(high-low>0.00001){//problem request
        mid=(low+high)/2;
        if(can_feed(1,mid,adj,liquidn))
            high=mid;
        else
            low=mid;
    }
    printf("%.2f\n", mid);
}

int main(void){
    static edge *adj_list[MAX_NODES + 1] = {NULL};
    static int liquid_needed[MAX_NODES + 1];
    int num_nodes, i;
    int from_node, to_node, percentage, superpipe;
    //read edges and create adjlist
    scanf("%d", &num_nodes);
    for (i = 0; i < num_nodes - 1; i++){
        scanf("%d%d%d%d", &from_node, &to_node, &percentage, &superpipe);
        add_edge(adj_list, from_node, to_node, percentage, superpipe);
    }
    //read needed
    for (i = 1; i <= num_nodes; i++)
        scanf("%d", &liquid_needed[i]);

    solve(adj_list, liquid_needed);//trova sol general
	return 0;
}