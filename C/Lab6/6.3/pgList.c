#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"
#define NOMEFILEOUT "out.txt"

typedef struct nodoPg_s {
  pg_t pg;
  struct nodoPg_s *next;
} nodoPg_t, *link;

struct pgList_s{
    link headPg, tailPg;
    int nPg;
};

link newNode(pg_t pg, link next){
    link x;
    x = malloc(sizeof(*x));
    x->pg = pg;
    x->next = next;
    return x;
}

pgList_t pgList_init(){
    pgList_t pgList;
    pgList = malloc(sizeof(*pgList));
    pgList->nPg = 0;
    return pgList;
}

void pgList_free(pgList_t pgList){
    link i, succ;
    for(i=pgList->headPg; i!=NULL; i=succ){
        succ =  i->next;
        pg_clean(&i->pg);
        free(i);
    }
}

void pgList_read(FILE *fp, pgList_t pgList){
    pg_t *tmp;

    while(pg_read(fp, tmp)){
        pgList_insert(pgList, *tmp);
        pgList->nPg++;
    }

}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    fp = fopen(NOMEFILEOUT, "w");
    link x;

    if(fp==NULL){
        printf("FILE NON TROVATO!");
        return;
    }

    for(x=pgList->headPg; x!=NULL; x = x->next){
        pg_print(fp, &x->pg ,invArray);
    }

}


void pgList_insert(pgList_t pgList, pg_t pg){
    link n;
    n = newNode(pg, NULL);
    if(pgList->headPg==NULL){
        pgList->tailPg = n;
        pgList->headPg = n;
    }
    else{
        pgList->tailPg->next = n;
        pgList->tailPg = n; //inserisco in coda
    }
}

void pgList_remove(pgList_t pgList, char* cod){
    link i, p;

    for(i=pgList->headPg, p=NULL; i!=NULL; p=i, i=i->next){
        if(strcmp(i->pg.cod,cod)==0){
            if(i==pgList->headPg && i==pgList->tailPg)
                pgList->headPg = pgList->tailPg = NULL;
            else if (i == pgList->headPg)
                pgList->headPg = i->next;
            else if (i == pgList->tailPg){
                pgList->tailPg = p;
                p->next = NULL;
            }
            else 
                p->next = i->next;
            pg_clean(&i->pg);
            free(i);
            break;
        }
    }
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
    link i;

    for(i = pgList->headPg; i!=NULL; i=i->next){
        if(strcmp(i->pg.cod,cod)==0)
            return (&i->pg);
    }
    return NULL;
}

