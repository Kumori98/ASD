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
    int i, j;
    if (g == NULL)
        return NULL;
    g->st = STinit(V);
    g->nodi = malloc (V*sizeof(nodo));

    g->madj = malloc (V*sizeof(int *));
    for (i=0; i < V; i++)
      g->madj[i] = malloc(V * sizeof(int));
    for (i=0; i < V; i++)
      for (j=0; j < V; j++)
        g->madj[i][j] = -1; //inizializzo tutto a -1

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

int GRAPHnumV(Graph g) {
  return g->V;
}

int GRAPHnumE(Graph g) {
  return g->E;
}

int dfsR(Graph g, int *time, int *pre, int *post, int start){
  int i, result = 0;
  pre[start] = (*time)++;
  for (i=0; i<g->V && result==0;i++){
    if (g->madj[start][i] != -1){
      if(pre[i] == -1) //vertice non ancora scoperto
        result = dfsR(g, time, pre, post, i);
      else  //se ho un arco che punta ad un vertice che è già stato scoperto
        if(post[i] == -1) //ma non ancora completato
          return 1; //ho trovato un arco back, cioè un ciclo
    }
  }
  post[start] = (*time)++;
  return result;
}

int GRAPHdfs(Graph g){
  int *pre, *post, i, *time=0;
    if (g == NULL)
      return -1;
    if (g->madj == NULL)
      return -1;

  pre = malloc(g->V*sizeof(int));
  post = malloc(g->V*sizeof(int));    
  for(i=0;i<g->V;i++){
    pre[i] = -1;
    post[i] = -1;
  }

  for(i=0;i<g->V;i++){
    if (pre[i] == -1) //vertice non ancora esplorato
      if(dfsR(g, time, pre, post, i))
        	return 1; //ho trovato un ciclo
  }

  free(pre);
  free(post);
  return 0; //ciclo non trovato
}

int GRAPHwt(arco_t *vE, int *sol, int k){
  int i, tot=0;
  for (i=0; i<k; i++)
    tot +=vE[sol[i]].peso;
  return tot;
}

void GRAPHremoveE(Graph g, arco_t e) {
  int v = e.v, w = e.w;
  if (g->madj[v][w] != -1)
    g->E--;
  g->madj[v][w] = -1;
}

void  GRAPHinsertE(Graph g, arco_t e) {
  int v = e.v, w = e.w, wt = e.peso;
  g->madj[v][w] = wt;
  g->E++;
}