#include <stdlib.h>
#include <stdio.h>
#include "ST.h"

typedef struct G *Graph;

typedef struct{
     int v, w, peso;
    }arco_t;

typedef struct{
     char nome[MAXLEN];
}nodo;

arco_t  EDGEcreate(int v, int w, int wt);
Graph GRAPHinit(int V);
Graph GRAPHload(FILE *fp);
void  GRAPHfree(Graph g);
void  GRAPHedges(Graph g, arco_t *a);
void  GRAPHstore(Graph g, FILE *fp);


