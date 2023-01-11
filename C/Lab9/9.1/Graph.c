#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ST.h"
#include "Graph.h"

struct G{
    ST_t st;
    int **madj;
    nodo *nodi;
    int V, E;
};

Graph GRAPHinit(int V){
    Graph g = malloc (sizeof(*g));
    if (g == NULL)
        return NULL;
    g->st = STinit(V);
    g->nodi = malloc (V*sizeof(nodo));
    return g;
}

Graph GRAPHload(FILE *fp){
    Graph g;
    int V, nE=0, id_1, id_2, peso, i;
    arco_t E;
    char src[MAXLEN], dest[MAXLEN];
    
    fscanf(fp, "%d", &V);
    g = GRAPHinit(V);
    if(g==NULL)
        return NULL;
    for(i=0; i<V; i++){
        fscanf(fp, "%s", &g->nodi[i].nome);
        STinsert(g->st,g->nodi[i].nome,i);
    }
    while(fscanf(fp, "%s %s %d", src, dest, peso)!=EOF){
        id_1 = STsearch(g->st, src);
        id_2 = STsearch(g->st, dest);
        if(id_1!= id_2){
            g->madj[id_1][id_2] = peso;
            g->E++;
        }
    }
    return g;
}

void GRAPHfree(Graph g){
    int i;
    for (i=0; i<g->V; i++)
        free(g->madj[i]);
    free(g->madj);
}
arco_t EDGEcreate(int v, int w, int wt) {
  arco_t e;
  e.v = v;
  e.w = w;
  e.peso = wt;
  return e;
}

void  GRAPHedges(Graph g, arco_t *a) {
  int v, w, E = 0;
  if (g == NULL)
    return;
  if (g->E <= 0)
    return;
  for (v=0; v < g->V; v++)
    for (w=0; w < g->V; w++)
      if (g->madj[v][w] != -1)
        a[E++] = EDGEcreate(v, w, g->madj[v][w]);
}

void GRAPHstore(Graph g, FILE *fp) {
  int i;
  arco_t *a;
  if (g == NULL)
    return;
  if (g->madj == NULL)
    return;

  a = malloc(g->E * sizeof(arco_t));
  if (a == NULL)
    return;

  GRAPHedges(g, a);

  fprintf(fp, "%d\n", g->V);
  for (i = 0; i < g->V; i++)
    fprintf(fp, "%s\n", &(g->nodi[i].nome));

  for (i = 0; i < g->E; i++)
    fprintf(fp, "%s  %s %d\n", &(g->nodi[a[i].v].nome), &(g->nodi[a[i].w]).nome, a[i].peso);
}