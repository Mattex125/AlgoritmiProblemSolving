//max-heap, we know n of elems so we use array form
//levels until last are all full from left to right
//every root must be higher than it's children

#define MR 100000
#include <stdio.h>
#include <stdlib.h>
//same material algo1
typedef struct heap_elem{
    int rindex; //identificativo scontrino
    int cost; //costo
}heap_elem;

void maxHeapInsert(heap_elem heap[], int *nelems, int ridx, int cost){//inserisco first node i found 
    int i;
    heap_elem tmp;//4 swapping
    heap[++(*nelems)]=(heap_elem){ridx,cost};
//gotta verify max prop
    i=*nelems;
    //while c'e genitore and costo figlio > padre
    while(i>1 && heap[i].cost > heap[i/2].cost){
        //swapping phase
        tmp=heap[i/2];
        heap[i/2]=heap[i];
        heap[i]=tmp;
        i=i/2; //go up
    }
}

heap_elem maxHeap(heap_elem heap[],int *nelems){
    heap_elem removed,tmp;
    int i,child;
    removed=heap[1];
    heap[1]=heap[(*nelems)--];
//setting again tree
    i=1;
    while (i*2 <= *nelems){//while i can go down
        child=i*2;
        if(child < *nelems && heap[child+1].cost > heap[child].cost)//ho due figli, ma dx maggiore, allora voglio usare lui
            child++;
        if(heap[child].cost > heap[i].cost){
            //swapping
            tmp=heap[i];
            heap[i]=heap[child];
            heap[child]=tmp;
            
            i=child;
        }
        else //i have finisced is ordered
            break;
    }
    return removed;
}

//min-heap
void minHeapInsert(heap_elem heap[], int *nelems, int ridx, int cost){
    int i;
    heap_elem tmp;
    heap[++(*nelems)]=(heap_elem){ridx,cost};
    i=*nelems;
    while(i>1 && heap[i].cost < heap[i/2].cost){ //differences: < instead of >
        tmp=heap[i/2];
        heap[i/2]=heap[i];
        heap[i]=tmp;
        i=i/2;
    }
}

heap_elem minHeap(heap_elem heap[],int *nelems){
    heap_elem removed,tmp;
    int i,child;
    removed=heap[1];
    heap[1]=heap[(*nelems)--];
    i=1;
    while (i*2 <= *nelems){
        child=i*2;
        if(child < *nelems && heap[child+1].cost < heap[child].cost) //differences: < instead of >
            child++;
        if(heap[child].cost < heap[i].cost){ //differences: < instead of >
            tmp=heap[i];
            heap[i]=heap[child];
            heap[child]=tmp;
            
            i=child;
        }
        else
            break;
    }
    return removed;
}

int main(){
    static int used[MR]={0};
    static heap_elem Mh[MR+1];
    static heap_elem mh[MR+1];
    int ndays,ridxToday;
    int receipidx=0;
    long long int totalp=0;
    int cost;
    int Mhelems=0, mhelems=0;
    heap_elem max,min;
    scanf("%d",&ndays);

    for(int i=0; i<ndays; i++){
        scanf("%d",&ridxToday);
        for(int j=0; j<ridxToday; j++){
        scanf("%d",&cost);
        //inserting in heaps
                maxHeapInsert(Mh,&Mhelems,receipidx,cost);
                minHeapInsert(mh,&mhelems,receipidx,cost);
                receipidx++;
        }
        max=maxHeap(Mh,&Mhelems);
        while(used[max.rindex])//controlla non sia gia stato usato
            max=maxHeap(Mh,&Mhelems);

        min=minHeap(mh,&mhelems);
        while(used[min.rindex])//controlla non sia gia stato usato
            min=minHeap(mh,&mhelems);

        used[max.rindex]=1;//lo uso
        used[min.rindex]=1;//lo uso

        totalp=totalp+(max.cost-min.cost);
    }
    printf("%lld\n", totalp);
    return 0;
}