#define MAXL 31
#include "ST.h"

typedef struct arco{ 
    int v, w, peso;
    }arco;

typedef struct G* grafo;

grafo GRAPHinit(int nV);
void  GRAPHinsertE(grafo g, int v, int w, int peso);
void  GRAPHstore(grafo g, ST st);
int   GRAPH3vertici(grafo g, int x, int y, int z);
void   GRAPHmat2list(grafo g);
void  GRAPHfree(grafo g);