#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "ST.h"

#define MAXL 31

int main(int argc, char **argv){
    Graph g = NULL;
    FILE *fp;

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

    

}