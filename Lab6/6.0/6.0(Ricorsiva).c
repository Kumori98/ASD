//ogni volta calcolo ogni singolo sottoproblema
#include <stdio.h>
#include <stdlib.h> 
#define NOMEFILE "val2.txt"

int solveR(int *val, int *opt, int n, int k);
void solve(int *val, int n);
int max(int n1, int n2);

int main(){
    int *val, n_antenne, i;
    FILE *fp;

    fp = fopen(NOMEFILE, "r");
    fscanf(fp, "%d", &n_antenne);

    val = (int *) malloc((n_antenne+1)*sizeof(int));
    
    val[0] = 0;
    for(i=1; i<(n_antenne+1); i++){
        fscanf(fp, "%d", &val[i]);
    }

    solve(val, n_antenne+1);

    return 0;

}

int solveR(int *val, int *opt, int n, int k){
    if(k==0)
        return 0;
    if(k==1){
        opt[k] = val[1];
        return opt[k];
    }
    opt[k] = max(solveR(val, opt, n, k-1), solveR(val, opt, n, k-2)+val[k]);
    return opt[k];
}

void solve(int *val, int n){
    int *opt;

    opt = (int *) calloc(n+1,sizeof(int)); //opt[i] contiene il numero di abitanti massimo considerando le antenne<=i
    printf("La massima popolazione coperta e' %d", solveR(val, opt, n, n));
}

int max(int n1, int n2){
    if(n1>=n2)
        return n1;
    return n2;
}