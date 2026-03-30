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
double solve_k(int num[], double prize[], int nschemas, double unitprize, int n2buy,double memo[]) //num appe per prize, sono le promo
{
    double best, result;
    if (memo[n2buy]!=-1)
        return memo[n2buy];
    if(n2buy==0) //actually not needed, it's inside a memo!=-1
        return 0;
    else{
        result=solve_k(num,prize,nschemas,unitprize,n2buy-1,memo); //IP compro tutte piu una mela, dopo verifica diversamente
        best=result+unitprize; //best og n-1 mele +1 unitaria
        for(int j=0; j<nschemas; j++){//controllo tutte le offerte
            if(n2buy-num[j]>=0) //che possa doverle comprare, CERCO SOLO CON ESATTAMENTE K mele 2buy
                result=solve_k(num,prize,nschemas,unitprize,n2buy-num[j],memo);//tutte meno quelle che assumo di mangiare
             //all of these per every prize scheme
            best=MIN2(best,result+prize[j]); //the lowest with that i had and best +prize this choose       
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

double solve(int num[], double prize[], int nschemas, double unitprize, int n2buy,double *memo){ //at least n apples
    //prezzo minimo con k, k+1 until all offers
    double best;
    int i;
    //try with k
    best=solve_k(num,prize,nschemas,unitprize,n2buy,memo);
    for (i=n2buy+1;i<=n2buy+max_array(num,nschemas);i++){ //when i stop:io ho pensato a n2buy+max in num: quelle che compro+la promo max
        best=MIN2(best,solve_k(num,prize,nschemas,unitprize,i,memo) );
    }
    return best;
}


int main(){
    double UnitPrize;
    int Nschemas;
    int Ncases=1;
    while(scanf("%lf %d",&UnitPrize,&Nschemas)!= EOF)
    {
        int NumApples[Nschemas];
        double PromoPrize[Nschemas];
        for (int i=0;i<Nschemas;i++)
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
        for (int i = 1; i < maxKtoTest; i++)
            memo[i] = -1;        
        memo[0]=0.00;
        while(head!=NULL){
            double result=solve(NumApples,PromoPrize,Nschemas,UnitPrize,head->n,memo);
            printf("Buy %d for $%.2lf\n",head->n,result);
            NodeOfBuys *placeholderDeallocator=head;
            head=head->next;
            free(placeholderDeallocator);
            }
        free(memo); //i free memo on every test to allocate a new one
    }   
    return 0;
}