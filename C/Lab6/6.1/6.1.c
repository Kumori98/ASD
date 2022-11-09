#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "att1.txt"

typedef struct attivita{
    int inizio, fine, durata;
} attivita_t;

attivita_t *carica_attivita(FILE *fp,attivita_t *attivita, int n_attivita);
int migliore_attivita(int pos, attivita_t *attivita, int *sol, int n_attivita, int *b_sol, int c_val, int max);
void attSel(int N, attivita_t *v); 
int isvalid(int *sol, int pos, attivita_t * v);



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

    attivita = carica_attivita(fp, attivita, n_attivita);

    attSel(n_attivita, attivita);

    return 0;

}

attivita_t *carica_attivita(FILE *fp,attivita_t *attivita, int n_attivita){
    int i;

    for(i=0; i<n_attivita; i++){
        fscanf(fp,"%d %d", &attivita[i].inizio, &attivita[i].fine);
        attivita[i].durata = attivita[i].fine - attivita[i].inizio;
    }

    return attivita;
}

void attSel(int N, attivita_t *v){
    int durata_max=0, *sol, *b_sol, i;

    sol = (int *) malloc(N*sizeof(int));
    b_sol = (int *) malloc(N*sizeof(int));
    durata_max = migliore_attivita(0, v, sol, N, b_sol, durata_max, durata_max);

    printf("L'insieme migliore di attivita' ha durata %s ed è formata da: \n", &durata_max);

    for(i=0; i<N; i++){
        if(b_sol[i]==1)
            printf("(%d,%d)", v[i].inizio, v[i].fine);
    }

    free(b_sol);;
    free(sol);
}

int migliore_attivita(int pos, attivita_t *attivita, int *sol, int n_attivita, int *b_sol, int c_val, int max){
    int j;
    if(pos >= n_attivita){
        if(c_val > max){
            max = c_val;
            for(j=0; j<n_attivita; j++)
                b_sol[j] = sol[j];
        }
        return max;
    }

    

    if(!isvalid(sol, pos, attivita)){
        sol[pos]=0;
        max = migliore_attivita(pos+1, attivita, sol, n_attivita, b_sol, c_val, max);
        return max;
    }

    sol[pos]=1;
    c_val = c_val + attivita[pos].durata;
    max = migliore_attivita(pos+1, attivita, sol, n_attivita, b_sol, c_val, max);

    sol[pos]=0;
    c_val = c_val - attivita[pos].durata;
    max = migliore_attivita(pos+1, attivita, sol, n_attivita, b_sol, c_val, max);

    return max;
}

int isvalid(int *sol, int pos, attivita_t * v){
    int i;

    for(i=0; i<pos; i++){
        if((v[i].inizio < v[pos].fine) && (v[pos].inizio < v[i].fine) && sol[i]==1)
            return 0;
    }
    return 1;
}
