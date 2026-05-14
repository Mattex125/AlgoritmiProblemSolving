#include <stdio.h>
#include <stdlib.h>

//DONT PASSED TEST
int tryCombination(int S[]);
void answer(int S[], int D[]);

void subprob1(int n,int *s,int *d){

    int prev = tryCombination(s);
    
    for(int i = 0; i < n; i++) {
    s[i]^=1;
    int result=tryCombination(s);
    if(result==-1)
        break;
    if(result>prev)
        prev=result;
    else
        s[i]^=1;
    }
}

void subprob2(int n, int *s, int *d){//do d
    //for (int i=0; i<n; i++){
    //    s[i]=0;
    //    d[i]=-1;
    //}
    for(int i=0; i<n; i++){//per every switch i, find which door it opens
        s[i]^=1;  //xor 0->1
        int result=tryCombination(s);
            d[i]=result; //switch i opens door result(the one that changed)
        s[i]^=1;  //xoe 1->0
    }       
}

//bad order of solving subproblems killled me
/*
void dod(int *s, int *d, int from, int to){
    for(int i=from; i<to; i++){
        s[i]^=1;
        int result=tryCombination(s);
        d[i]=result; //switch i opens door result(the one that changed)
        s[i]^=1;
    }
}
void solve(int n, int *s, int *d){
    int prevresult=0,result;//parto con zero giuste
    for(int i=0; i<n; i++){//per every switch i
    result=tryCombination(s);
    if(result>prevresult){
        if(result>prevresult)
            dod(s,d,i,result-prevresult);//solve the assotiations, before i
        d[i]=result;
    }
    else{ //means it's equal -->i have d for this
        s[i]^=1;  //xor 0->1
        }   
    prevresult=result;
    }
}*/

// Phase A: determine the correct combination S (which switches should be 1)
// Greedy: try toggling each switch; keep the toggle if it moves the
// "first-closed" index to the right (or solves the cave).
void determine_solution(int n, int *s){
    int prev = tryCombination(s);
    
    // If already at goal (-1 = all open), stop immediately
    if (prev == -1) return;

    for(int i = 0; i < n; i++) {
        s[i] ^= 1; // try flip
        int result = tryCombination(s);
        if (result == -1) {
            // all doors open; keep and finish
            return;
        }
        if (result > prev) {
            // improvement, keep flipped and update prev
            prev = result;
        } else {
            // no improvement, revert
            s[i] ^= 1;
        }
    }
}

// Phase B: locate which switch controls each door
// This routine starts from all-zero state and finds, for each door j=0..n-1,
// the single switch p that opens door j. It sets d[p]=j and leaves s filled
// so that doors <=(j) remain open for subsequent iterations.
void find_mapping(int n, int *s, int *d){
    // start from all zeros base
    for (int i = 0; i < n; ++i) s[i] = 0;

    char used[n];
    for (int i = 0; i < n; ++i) used[i] = 0;

    // Simple deterministic linear search: for each door j, try each unused
    // switch p by toggling it; if toggling moves the first-closed door past j
    // (or opens all doors), then p controls door j. Leave s[p] toggled so
    // door j remains open for subsequent iterations.
    for (int j = 0; j < n; ++j) {
        int found = 0;
        for (int p = 0; p < n; ++p) {
            if (used[p]) continue;
            s[p] ^= 1;
            int r = tryCombination(s);
            int rr = (r == -1) ? n : r;
            if (rr > j) {
                d[p] = j;
                used[p] = 1;
                // leave s[p] as set (so door j stays open)
                found = 1;
                break;
            }
            // not the controller, revert
            s[p] ^= 1;
        }
        if (!found) {
            // Should not happen; fallback: pick any unused switch
            for (int p = 0; p < n; ++p) if (!used[p]) { d[p] = j; used[p]=1; s[p]=1; break; }
        }
    }
}

void realsol(int n, int *s, int *d){
    // Phase A: determine solution bits and store in s
    for (int i = 0; i < n; ++i) s[i] = 0;
    determine_solution(n, s);

    // Phase B: given the correct solution `s`, find mapping by flipping
    // each switch individually and observing the first-closed door.
    int *tmp = malloc(sizeof(int)*n);
    for (int i = 0; i < n; ++i) tmp[i] = s[i];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) tmp[j] = s[j];
        tmp[i] ^= 1;
        int r = tryCombination(tmp);
        d[i] = r;
    }
    free(tmp);
}

void exploreCave(int n){
    int *s = calloc(n,sizeof(int));
    int *d = malloc(sizeof(int) * n);
    realsol(n,s,d);
    answer(s,d);
    free(s);
    free(d);
}

int main(){//placeholder x makefile
    return 0;
}