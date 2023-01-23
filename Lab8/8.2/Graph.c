#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "ST.h"
#include "Graph.h"

typedef struct node *link;
struct node { int v; int peso; link next; };

struct G { int **madj; link *ladj; int V; int E;};

link newNode(int v, int peso, link next) {
  link x;
  x = malloc(sizeof *x);
  x->v = v;
  x->peso = peso;
  x->next = next;
  return x;
}

grafo GRAPHinit(int V) {
    int i, j;
    int **madj = malloc(V*sizeof(int*));
    
    grafo g = malloc(sizeof(*g));
    g->V = V;
    g->E = 0;
    for (i=0; i < V; i++) 
        madj[i] = malloc(V * sizeof(int));
    for (i=0; i < V; i++)
        for (j=0; j < V; j++)
            madj[i][j] = 0;
    g->madj = madj;
    g->ladj = NULL;
  return g;
}

void GRAPHinsertE(grafo g, int v, int w, int peso) {
    if (g->madj[v][w] == 0)
        g->E++;
    g->madj[v][w] = peso;
    g->madj[w][v] = peso; //essendo non orientato
}

void GRAPHstore(grafo g, ST st) {
  int i, j;
  for (i=0; i<g->V; i++) {
    STdisplayByIndex(st, i);
    printf("\n");
    for (j=0; j<g->V; j++) {
      if (g->madj[i][j]) {
        printf(" ");
        STdisplayByIndex(st, j);
        printf(" ");
      }
    }
    printf("\n");
  }
}

void GRAPHfree(grafo g) {
  int v;
  link tmp, x;
  for (v=0; v<g->V; v++)
    free(g->madj[v]);
  free(g->madj);

    for (v=0; v < g->V; v++)
      for (x=g->ladj[v]; x != NULL;) {
        tmp = x;
        x = x->next;
        free(tmp);
      }
    free(g->ladj);
    free(g);
}

int GRAPH3vertici(grafo g, int x, int y, int z){
    if(g->madj[x][y]!=0 && g->madj[x][z]!=0 && g->madj[z][y]!=0)
        return 1;
    return 0;
}

void GRAPHmat2list(grafo g) {
  int v, w;
  g->ladj = calloc(g->V, sizeof(link));

  for (v=0; v<g->V; v++)
    for (w=0; w<g->V; w++)
      if (g->madj[v][w]>0) {
        g->ladj[v] = NEW(w, g->madj[v][w], g->ladj[v]);
        g->ladj[w] = NEW(v, g->madj[v][w], g->ladj[w]);
      }
}

