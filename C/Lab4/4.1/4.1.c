#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "grafo.txt"

typedef struct arco{
    int nodo1;
    int nodo2;
}arco_t;

typedef struct grafo{
    int N, E;
    arco_t *archi;
}grafo_t;


void carica_grafo(FILE *fp, grafo_t *grafo);
void vertex_cover(int pos, grafo_t *grafo, int *sol);
void is_valid(grafo_t *grafo, int *sol);

int main(){
    grafo_t *grafo;
    FILE *fp;
    int *sol;

    fp = fopen(NOMEFILE,"r");
    if(fp == NULL){
        printf("File non trovato");
        return -1;
    }

    carica_grafo(fp, grafo);
    fclose(fp);
    
    sol = (int *) malloc(grafo->N*sizeof(int));
    vertex_cover(0, grafo, sol);

    free(sol);
    free(grafo->archi);

    return 0;
}

void carica_grafo(FILE *fp, grafo_t *grafo){
    int i;
    fscanf(fp, "%d %d", &grafo->N, &grafo->E);
    grafo->archi = (arco_t *) malloc(grafo->E*sizeof(arco_t));
    for(i=0; i<grafo->E; i++){
        fscanf(fp, "%d %d", &grafo->archi[i].nodo1, &grafo->archi[i].nodo2);
    }
}

void vertex_cover(int pos, grafo_t *grafo, int *sol){
    int i;
    if(pos>=grafo->N){ //ho una soluzione, e' valida?
        is_valid(grafo, sol);
        return;    
    }
    sol[pos]=0;
    vertex_cover(pos+1, grafo, sol);
    sol[pos]=1;
    vertex_cover(pos+1, grafo, sol);
}

void is_valid(grafo_t *grafo, int *sol){
    int check=0;
    int i, j;


    for(j=0; j<grafo->E; j++){
        for(i=0;i<grafo->N;i++){
            if(((grafo->archi[j].nodo1 == i) || (grafo->archi[j].nodo2 == i)) && (sol[i] == 1)){
                check++;
                break;
            }
        }
    }

    for(i=0; i<grafo->E; i++){ //check finale per vedere se è valida
        if(check != grafo->E )
            return;
    }
    
    printf("{ \t");
    for(i=0;i<grafo->N;i++){
        if(sol[i]==1)
        printf("%d \t", i);
        }
    printf("} \n");
}



