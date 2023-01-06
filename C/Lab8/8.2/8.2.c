#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include<string.h>
#define MAXL 31
#define NOMEFILE "grafo.txt"
#include "Item.h"
#include "ST.h"
#include "Graph.h"

grafo carica_grafo(FILE *fp, ST st);
int stampa_menu();

int main(){
    grafo g = NULL;
    FILE *fp;
    ST st = STinit(1);
    int scelta, nodo1, nodo2, nodo3;
    char node[MAXL];

    fp = fopen(NOMEFILE, "r");
    if(fp == NULL){
        printf("Errore lettura file");
        return -1;
    }

    g = carica_grafo (fp, st);

    do{
        scelta = stampa_menu();
        switch (scelta){
            case 1: 
                GRAPHstore(g, st);
                break;

            case 2: 
                printf("Inserisci il primo nodo: ");
                KEYscan(node);
                nodo1 = STsearch(st, node);
                printf("Inserisci nome secondo nodo: ");
                KEYscan(node);
                nodo2 = STsearch(st, node);
                printf("Inserisci nome terzo nodo: ");
                KEYscan(node);
                nodo3 = STsearch(st, node);
                if ((nodo1 != -1) && (nodo2 != -1) && (nodo3 != -1) && GRAPH3vertici(g, nodo1, nodo2, nodo3))
                    printf("Il sottografo e' completo\n");
                else
                    printf("Il sottografo non e' completo");
                break;
            
            case 3:
                GRAPHmat2list(g);
                GRAPHstore(g, st);
                break;

            case 4:
            break;

            default:
            printf("Scelta non valida\n");
        }
    } while(scelta != 4);

    STfree(st);
    GRAPHfree(g);
    return 1;
}

grafo carica_grafo(FILE *fp, ST st){
    Item item;
    char elem1[MAXL], rete1[MAXL], elem2[MAXL], rete2[MAXL];
    grafo g;
    int peso, id1, id2;
    g = GRAPHinit(STcount(st));

    while(fscanf(fp, "%s %s %s %s %d\n", elem1, rete1, elem2, rete2, &peso) != EOF){
        item = ITEMload(elem1, rete1);
        id1 = STsearch(st, KEYget(&item));
        if (id1 == -1)
            STinsert(st, item);
        item = ITEMload(elem2, rete2);
        id2 = STsearch(st, KEYget(&item));
        if (id2 == -1)
            STinsert(st, item);
        if (id1 != id2 && id1 >= 0 && id2 >= 0)
            GRAPHinsertE(g, id1, id2, peso);        
  }
  return g;
}

int stampa_menu(){
    int scelta;
    printf("Si scelga l'operazione tra le seguenti inserendo il relativo indice:\n");
    printf("1: Stampa grafo in ordine alfabetico;\n");
    printf("2: Verifica se 3 vertici sono adiacenti a coppie;\n");
    printf("3: Rappresentazione a lista di adiacenza;\n");
    printf("4: Esci\n");
    scanf("%d", &scelta);
    return scelta;
}
