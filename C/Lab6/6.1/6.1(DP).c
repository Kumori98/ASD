#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "att2.txt"

typedef struct attivita{
    int inizio, fine, durata;
} attivita_t;

attivita_t *carica_attivita(FILE *fp,attivita_t *attivita, int n_attivita);
int isvalid(attivita_t a1, attivita_t a2);
void solveDP(attivita_t *attivita, int n);
int max(int n1, int n2);
int maxComp(attivita_t *attivita, int k);
void ordina(attivita_t *attivita, int n);
void swap(int i1, int i2, attivita_t *attivita);
void displaySol(int *opt,attivita_t *attivita,int n);



int main(){
    FILE *fp;
    attivita_t *attivita;
    int n_attivita;

    fp = fopen(NOMEFILE, "r");

    if(fp==NULL){
        printf("File non trovato!\n");
        return -1;
    }

    fscanf(fp, "%d", &n_attivita);

    attivita = (attivita_t *) malloc((n_attivita+1)*sizeof(attivita_t));
    attivita = carica_attivita(fp, attivita, n_attivita);

    ordina(attivita, n_attivita+1);
    solveDP(attivita, n_attivita+1);

    return 0;

}

attivita_t *carica_attivita(FILE *fp,attivita_t *attivita, int n_attivita){
    int i;

    attivita[0].durata=0;
    for(i=1; i<n_attivita+1; i++){
        fscanf(fp,"%d %d", &attivita[i].inizio, &attivita[i].fine);
        attivita[i].durata = attivita[i].fine - attivita[i].inizio;
    }

    return attivita;
}

int isvalid(attivita_t a1, attivita_t a2){
        if((a1.inizio < a2.fine) && (a2.inizio < a1.fine))
            return 0;
    return 1;
}

void solveDP(attivita_t *attivita, int n){
    int *opt, i;

    opt = (int *) calloc((n+1),sizeof(int));
    opt[1] = attivita[1].durata;
    for(i=2; i<n; i++){
        if(isvalid(attivita[i-1], attivita[i]))
            opt[i] = opt[i-1] + attivita[i].durata;
        else
            opt[i] = opt[maxComp(attivita, i)] + attivita[i].durata;
    }
    printf("(Soluzione con programmazione dinamica) ");
    printf("La combinazione di attivita' migliore e' lunga: %d\n", opt[n-1]);
    displaySol(opt, attivita, n);
}

int solveR(attivita_t *attivita, int *opt, int n, int k){
    if(k==0)
        return 0;
    if(k==1){
        opt[k] = attivita[k].durata;
        return opt[k];
    }
    if(isvalid(attivita[k], attivita[k-1])){
        opt[k] = solveR(attivita,opt,n,k-1) + attivita[k].durata;
        return opt[k];
    }
    else{
        opt[k] = max(solveR(attivita,opt,n,k-1),(solveR(attivita,opt,n,maxComp(attivita, k))+(attivita[k].durata)));
        return opt[k];
    }
     
}

int max(int n1, int n2){
    if(n1>=n2)
        return n1;
    return n2;
 }

 int maxComp(attivita_t *attivita, int k){ //attivita precedente compatibile massima
    int i;
    for(i=k-2; i>0; i--){
        if(isvalid(attivita[i],attivita[k]))
            return i;
    }
    return i;
}

void ordina(attivita_t *attivita, int n){
    int i;
    for(i=2; i<n; i++){
        if(attivita[i-1].fine>attivita[i].fine)
            swap((i-1), i, attivita);
        else if(attivita[i-1].fine==attivita[i].fine){
            if(attivita[i-1].inizio<attivita[i].inizio)
                swap((i-1), i, attivita);
        }
    }
} 

void swap(int i1, int i2, attivita_t *attivita){
    attivita_t tmp;
    tmp.durata = attivita[i1].durata;
    tmp.fine = attivita[i1].fine;
    tmp.inizio = attivita[i1].inizio;

    attivita[i1].durata = attivita[i2].durata;
    attivita[i1].fine = attivita[i2].fine;
    attivita[i1].inizio = attivita[i2].inizio;

    attivita[i2].durata = tmp.durata;
    attivita[i2].fine = tmp.fine;
    attivita[i2].inizio = tmp.inizio;
}

void displaySol(int *opt,attivita_t *attivita,int n){
    int i, *sol, val, j;
    sol = (int *) calloc(n,sizeof(int));
    i=n-1;  //parto dalla fine

    while(i>=1){ 
        if(opt[i] == opt[i-1]){ // non ho preso quell'attività
            sol[i] = 0;
            i--;
        }
        else {
            j=i;
            while(opt[j] != opt[i-1] + attivita[j].durata){
                i--;
                sol[i]=0;
            }
            sol[j] = 1;
            i--;
        }
    }
    for(i=1; i<n; i++){
        if(sol[i])
            printf("{%d,%d} ", attivita[i].inizio, attivita[i].fine);
    }
}