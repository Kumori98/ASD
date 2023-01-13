#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "ST.h"

#define MAXL 31
void powerset(Graph g);
int comb_sempl(int pos, Graph g, arco_t *vE, int E, int *sol, int start, int k, int *bestSol, int *bestWt);

int main(int argc, char **argv){
    Graph g = NULL;
    FILE *fp;
    int V;

    if(argc != 2){
        printf("FILE MANCANTE\n");
        return -1;
    }

    fp = fopen(argv[1], "r");
    if (fp==NULL){
        printf("ERRORE FILE NON TROVATO\n");
        return -1;
    }

    g = GRAPHload(fp);

    printf("Grafo iniziale\n");
    GRAPHstore(g, stdout);

    if(!GRAPHdfs(g)){ //La funzione ritorna 1 se ho trovato almeno un arco back(Ho un ciclo)
        printf("Il grafo iniziale è già un DAG\n");
    }
    else{
        calcolaSet(g);
        printf("La soluzione ottima è:\n");
        GRAPHstore(g, stdout);
        printf("\n");
    }

    V = GRAPHgetNumV(g);

    DAGts(g);

    printf("Le distanze massime sono: \n");

    DAGmaxDis(g);char *STsearchByIndex(ST_t st, int index);

    GRAPHfree(g);
}

void powerset(Graph g){
    int E = GRAPHnumE(g), V = GRAPHnumV(g), *sol, flag=0, k, *bestSol, bestWt=0, i;
    arco_t *vE = malloc(E*sizeof(arco_t));

     GRAPHedges(g, vE);

    for(k=1; k<V-2 && flag==0; k++){
        sol = malloc(k*sizeof(int));
        bestSol = malloc(k*sizeof(int));
        flag = comb_sempl(0, g, vE, E, sol, 0, k, bestSol, &bestWt);
        free(sol);
        if(flag==0)
            free(bestSol);
    }

    for (i=0; i<k-1; i++)
            GRAPHremoveE(g, vE[bestSol[i]]);

    free(bestSol);
}

int comb_sempl(int pos, Graph g, arco_t *vE, int E, int *sol, int start, int k, int *bestSol, int *bestWt){
    int i, flag=0;
    if(pos>=k){
        for(i=0; i<k; i++)
            GRAPHremoveE(g, vE[sol[i]]);
        if(!GRAPHdfs(g) && GRAPHwt(vE,sol,k)>bestWt){
            bestWt = GRAPHwt(vE,sol,k);
            for(i=0; i<k; i++)
                bestSol[i] = sol[i];
            flag = 1;
        }
        for (i=0; i<k; i++)
            GRAPHinsertE(g, vE[sol[i]]);
        return flag;
    }
    for (i=start; i<k; i++) {
        sol[pos] = i;
        flag = comb_sempl(pos++, g, vE, E, sol, i+1, k, bestSol, bestWt);
    }   
  return flag;
}

