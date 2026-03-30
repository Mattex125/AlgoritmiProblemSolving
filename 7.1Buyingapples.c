#include <stdio.h>
#include <stdlib.h>

typedef struct promo { //promos
    double prize;
    int nobjs;
    struct promo* next;
} promo;

typedef struct node { //lista di k to test
    int val;
    struct node* next;
} node; 

int nelems = 0;

/* ---- memo ---- */
double *memo = NULL;
int MEMSIZE = 0;

promo* addPromo(promo* head, double prize, int nobjs) {
    promo* newnode = (promo*)malloc(sizeof(promo));
    newnode->prize = prize;
    newnode->nobjs = nobjs;
    newnode->next = head;
    nelems++;
    return newnode;
}

node* enqueue(node* head, int val) {
    node* newnode = (node*)malloc(sizeof(node));
    newnode->val = val;
    newnode->next = NULL;

    if (head == NULL)
        return newnode;

    node* tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    tmp->next = newnode;
    return head;
}

int maxNobjs(promo* head) {
    int max = 0;
    while (head != NULL) {
        if (head->nobjs > max)
            max = head->nobjs;
        head = head->next;
    }
    return max;
}

double solvek(promo* head, int nelems, double unit_price) {
    if (nelems <= 0)
        return 0;

    if (nelems < MEMSIZE && memo[nelems] >= 0)
        return memo[nelems];

    //ogni volta calcolo il prezzo tutti unit e poi cerco di migliorare, why dont use k-1? DONE
    double min = solvek(head, nelems - 1, unit_price) + unit_price;

    promo* curr = head;
    while (curr != NULL) {
        double val = curr->prize + solvek(head, nelems - curr->nobjs, unit_price);
        if (val < min)
            min = val;
        curr = curr->next;
    }

    if (nelems < MEMSIZE)
        memo[nelems] = min;

    return min;
}

void freePromoList(promo* head) {
    promo* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    nelems = 0;
}

void freeNodeList(node* head) {
    node* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

/* ---- main ---- */

int main() {
    double unit_price;
    int npromo;
    int case_num = 1;

    while (scanf("%lf %d", &unit_price, &npromo) == 2) { //read case

        promo* plist = NULL;
        node* list = NULL;

        for (int i = 0; i < npromo; i++) { //read all promos
            int nobj;
            double p;
            scanf("%d %lf", &nobj, &p);
            plist = addPromo(plist, p, nobj);
        }

        int x;
        while (scanf("%d", &x) == 1) {
            list = enqueue(list, x);
            int c = getchar();
            if (c == '\n' || c == EOF)
                break;
        }

        printf("Case %d:\n", case_num++);

        node* tmp = list;
        int maxobj = maxNobjs(plist);

        /* ---- alloc memo ---- */
        int maxk = 0;
        node* t2 = list;
        while (t2 != NULL) {
            if (t2->val > maxk)
                maxk = t2->val;
            t2 = t2->next;
        }

        MEMSIZE = maxk + maxobj + 1;
        memo = (double*)malloc(sizeof(double) * MEMSIZE);
        for (int i = 0; i < MEMSIZE; i++)
            memo[i] = -1;
        memo[0] = 0;

        while (tmp != NULL) {
            double min = 1e9;
            for (int k = tmp->val; k <= tmp->val + maxobj; k++) {
                double res = solvek(plist, k, unit_price);
                if (res < min)
                    min = res;
            }

            printf("Buy %d for $%.2f\n", tmp->val, min);

            tmp = tmp->next;
        }

        free(memo);
        freePromoList(plist);
        freeNodeList(list);
    }

    return 0;
}