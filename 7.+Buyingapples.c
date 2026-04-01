#include <stdio.h>
#include <stdlib.h>

#define MIN2(a,b) ((a) < (b) ? (a) : (b))

typedef struct NodeOfBuys
{
    struct NodeOfBuys *next;
    int n;
}NodeOfBuys;

void insertTail(NodeOfBuys **head, int value) {
    NodeOfBuys *newNode = malloc(sizeof(NodeOfBuys));
    newNode->n = value;
    newNode->next = NULL;
    
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    
    NodeOfBuys *curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newNode;
}

int max_list(NodeOfBuys *head) { //per trovare il numero max k da gestire
    int mx = 0;
    while (head != NULL) {
        if (head->n > mx) mx = head->n;
        head = head->next;
    }
    return mx;
}
//k and only k appples
double solve_k(int num[], double prize[], int nschemas, double unitprize, int n2buy,double memo[],int prev []) //num appe per prize, sono le promo
{
    double best, result;
    if (memo[n2buy]!=-1)
        return memo[n2buy];
    if(n2buy==0) //actually not needed, it's inside a memo!=-1
        return 0;
    else{
        result=solve_k(num,prize,nschemas,unitprize,n2buy-1,memo,prev); //IP compro tutte piu una mela, dopo verifica diversamente
        best=result+unitprize; //best og n-1 mele +1 unitaria
        prev[n2buy]=n2buy-1; //CI VA CHE OFFERTA HO DECISO DI PRENDERE
        for(int j=0; j<nschemas; j++){//controllo tutte le offerte
            if(n2buy-num[j]>=0) //che possa doverle comprare, CERCO SOLO CON ESATTAMENTE K mele 2buy
                result=solve_k(num,prize,nschemas,unitprize,n2buy-num[j],memo,prev);//tutte meno quelle che assumo di mangiare
             //all of these per every prize scheme
            if (best>result+prize[j]){ //if i got better offer
                best=result+prize[j];
                prev[n2buy]=n2buy-num[j];//prev[istanzathisproblem]=sol prima di questa
            }
        }
        return memo[n2buy]=best;
    }
}

int max_array(const int *arr, int n) {
    if (n <= 0) return 0; // or handle as error
    int mx = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > mx) mx = arr[i];
    }
    return mx;
}

double solve(int num[], double prize[], int nschemas, double unitprize, int n2buy,double *memo, int *whathaveichosen,int *bestk){ //at least n apples
    //prezzo minimo con k, k+1 until all offers
    double best;
    int i;
    //try with k
    best=solve_k(num,prize,nschemas,unitprize,n2buy,memo,whathaveichosen);
    *bestk=n2buy; //ip this k is the best offer
    for (i=n2buy+1;i<=n2buy+max_array(num,nschemas);i++){ //when i stop:io ho pensato a n2buy+max in num: quelle che compro+la promo max
        double newsol=solve_k(num,prize,nschemas,unitprize,i,memo,whathaveichosen);
        if(best>newsol){
            best=newsol;
            *bestk=i;
        }
    }
    return best;
}

void rebuild_sol(int k,int prev[]){
    printf("sol for %d elems is done via choosing\n",k);
    while(k > 0){
        int diff = k - prev[k];

        if(diff == 1)
            printf("1 apple\n");
        else
            printf("promo of %d apples\n", diff);

        k = prev[k];
    }
}

int main(){
    double UnitPrize;
    int Nschemas;
    int Ncases=1;
    while(scanf("%lf %d",&UnitPrize,&Nschemas)!=EOF)//more secure ==2
    {
        int NumApples[Nschemas];
        double PromoPrize[Nschemas];
        for (int i=0;i<Nschemas;i++)//read schemas
            scanf("%d %lf",&NumApples[i],&PromoPrize[i]);
        NodeOfBuys* head=NULL;
        char terminator=0;
        int nBuyTest;
        while(terminator!='\n'){
            scanf("%d%c",&nBuyTest,&terminator);
            insertTail(&head,nBuyTest);
        }
        printf("Case %d:\n",Ncases++); 
        int maxKtoTest=(max_array(NumApples, Nschemas)+max_list(head)+1);
        double* memo = malloc( maxKtoTest * sizeof(double));//test piu grande +1 pk c'e 0
        int *prev = malloc(maxKtoTest * sizeof(int)); //whatihave choosen is only per test so it gets restarted everytime
        for (int i = 1; i < maxKtoTest; i++){
            memo[i] = -1;
            prev[i] = -1;}
        memo[0]=0.00;
        while(head!=NULL){
            int bestk; //tells me what k has given best sol
            //prev[i]=ntimes i took that offer
            double result=solve(NumApples,PromoPrize,Nschemas,UnitPrize,head->n,memo,prev,&bestk);
            printf("Buy %d for $%.2lf\n",head->n,result);

            rebuild_sol(bestk,prev);

            NodeOfBuys *placeholderDeallocator=head;
            head=head->next;
            
            free(placeholderDeallocator);
            }
        free(memo); //i free memo on every test to allocate a new one
        free(prev);
    }   
    return 0;
}
/*
buildsol with memo
when memo is filled, prev is too!
prev has the optimal until now, so latest+this one
while objtoreach>0:
    diff=k-prev[k]
    diff has what i have choosen, in this case nmele shooped
    k=prev[k]

ip: if i need to buildsol is better dy bottom up
    */