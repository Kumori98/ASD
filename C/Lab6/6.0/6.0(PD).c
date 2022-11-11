#include <stdio.h>
#include <stdlib.h> 
#define NOMEFILE "val2.txt"

void solveDP(int *val, int n);
void displaySol(int *opt, int *val, int n);

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

    solveDP(val, n_antenne+1);

    return 0;

}


void solveDP(int *val, int n){
    int *opt, i;

    opt = (int *) calloc(n+1,sizeof(int)); //opt[i] contiene il numero di abitanti massimo considerando le antenne<=i
    opt[1] = val[1];
    for(i=2; i<n; i++){
        if(opt[i-1] > opt[i-2]+val[i])
            opt[i] = opt[i-1];
        else   
            opt[i] = opt[i-2] + val[i]; 
    }
    printf("La massima popolazione coperta e' %d con le antenne: \n", opt[n-1]);
    displaySol(opt, val, n);
}

void displaySol(int *opt, int *val, int n){ //l'elemento i-esimo appartiene(sol[i]=1) o no(sol[i]=0) alla soluzione?
    int i, j, *sol;
    sol = (int *) calloc((n),sizeof(int));
    sol[1] = 1; //assunzione iniziale poi modificabile
    i=n-1;  //parto dalla fine
    while(i>=2){ //mi fermo quando i=2
        if(opt[i] == opt[i-1]){ // non ho messo l'antenna in quella citta, non so nulla sulla citta precedente
            sol[i] = 0;
            i--;
        }
        else if(opt[i] == opt[i-2] + val[i]){ //ho messo l'antenna in quella citta, non avrò un'antenna nella citta precedente
            sol[i] = 1;
            sol[i-1] = 0;
            i-= 2;
        }
    }
    for(i=0; i<n; i++)
        if(sol[i])
            printf("Antenna %d che copre %d abitanti; \n", i, val[i]);
}
