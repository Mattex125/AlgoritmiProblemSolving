//leggere matrix adj e metterla in lista adj
#define MAXT 700
#define MOD 1000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct edge {
  int to_town, length;
  struct edge *next;
} edge;

void solve(edge *adjlist[], int ntowns, int store[]){
    static int done[MAXT+1][2];//n nodo e lo stato 0, 1 si cookie
    static int mind[MAXT+1][2];//min distances
    static int npaths[MAXT+1][2];//num min paths
    int i,j,state,found;
    int md, mtidx, minstateidx, olddist;
    edge *e;
    //inizializzo
    for (i = 1; i <= ntowns; i++) {
        for (state = 0; state < 2; state++) {
            done[i][state] = 0;
            mind[i][state] = -1;
            npaths[i][state] = 0;
        }
    }
    //start
    mind[1][0]=0;
    npaths[1][0]=1;
//search 4 less dist
    for(i=0; i<ntowns*2 ; i++){// doppio numero town, x ogni town 2 states
        md=-1;
        found=0;
        for(state=0; state<=1; state++){ //lookup 4 min
            for(j=1;j<=ntowns;j++){
                if(!done[j][state] && mind[j][state] >= 0){//se non è stato gia completato and è ragg
                    if(md==-1 || mind[j][state]<md){//not chosen next or a new nearby
                        //set new distance
                        md=mind[j][state];
                        //nearest (to be relaxed)
                        mtidx=j;
                        minstateidx=state;
                        found=1;
                    }
                }
            }
        }
        if(!found) //if cant explore i have finisced visit
            break;
        done[mtidx][minstateidx]=1; //set to done what we'll explore

//relaxation

        if(minstateidx==0 && store[mtidx]) //we are in a cookie store so we transit to tate one
        {
            olddist=mind[mtidx][1]; //old dist to cookie on that node
            if(olddist==-1 || olddist>md){
                mind[mtidx][1]=md;//i set min dist to get a cookie
                npaths[mtidx][1]=npaths[mtidx][0]%MOD;
            }
            else if(olddist==md){
                npaths[mtidx][1]=(npaths[mtidx][1]+npaths[mtidx][0])%MOD;
            }
        }
        else { //explore nearby
            e=adjlist[mtidx];
            while(e){//downscrolling list
                olddist=mind[e->to_town][minstateidx];
                int newdist = md + e->length;
                if(olddist == -1 || olddist > newdist){ //if i have better
                    //set new dist 
                    mind[e->to_town][minstateidx] = newdist;
                    npaths[e->to_town][minstateidx] = npaths[mtidx][minstateidx]%MOD;
                }
                else if(olddist == newdist){//new eq sol
                    npaths[e->to_town][minstateidx] = (npaths[e->to_town][minstateidx] + npaths[mtidx][minstateidx])%MOD;
                }
            e=e->next;
            }
        }
        
    }
    printf("%d %d\n",mind[ntowns][1],npaths[ntowns][1]);
    
}

int main(){
    static edge *adjlist[MAXT+1]={NULL};//graph, adjlist
    int i,ntowns,fromtown,totown,len;
    int nstores,storenum;
    static int stores[MAXT+1]={0};//stores bitmap
    edge *e;
    scanf("%d", &ntowns);
    for(fromtown=1; fromtown<=ntowns; fromtown++){//lista matrix adj to create adjlist
        for(totown=1; totown<=ntowns; totown++){
            scanf("%d", &len);
            if(len<=0) continue;
            e = malloc(sizeof(edge));
            if (e == NULL) {
                fprintf(stderr, "malloc error\n");
                exit(1);
            }
            //list add
            e->to_town = totown;
            e->length = len;
            e->next = adjlist[fromtown];
            adjlist[fromtown] = e;
        }
    }
    scanf("%d",&nstores);
    //set 1 when n store has cookies
    for(i=1;i<=nstores;i++){
        scanf("%d",&storenum);
        stores[storenum]=1;
    }
    solve(adjlist,ntowns,stores);
    //divido graph in 2, state 0 ma duplico tutto con state 1, sono passato da un town biscotti
    //per ogni nodo con cookies si collega a se ma con state diverso (senza poter tornare indietro) e il resto rimane uguale
    //e sparo disjtrak
    return 0;
}