#include "pgList.h"
#define NOMEFILE "pg.txt"
#define NOMEFILEOUT "out.txt"

typedef struct nodoPg *link;

typedef struct nodoPg{
    pg_t pg;
    link next;
} nodoPg_t;

struct pgList_s{
    link headPg, tailPg;
    int nPg;
};

pgList_t pgList_init(){
    pgList_t pgList;
    pgList = malloc(sizeof(pgList_t));
    pgList->nPg = 0;
    return pgList;
}

void pgList_free(pgList_t pgList){
    link i;
    for(i=pgList->headPg; i!=NULL; i=i->next){
        pg_clean(&i->pg);
    }
    free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList){
    fp = fopen(NOMEFILE, "r");
    pg_t *tmp;

    if(fp==NULL)
        return;

    while(pg_read(fp, tmp)){
        pgList_insert(pgList, *tmp);
        pgList->nPg++;
    }

    fclose(fp);
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

link newNode(pg_t pg, link next){
    link x;

    x = malloc(sizeof(link));
    x->pg = pg;
    x->next = next;
}

void pgList_insert(pgList_t pgList, pg_t pg){
    link n;
    n = newNode(pg, NULL);
    if(pgList->headPg==NULL){
        pgList->tailPg = n;
        pgList->headPg = n;
    }
    else{
        pgList->headPg = n; //inserimento in testa
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
            else if (i == pgList->tailPg)
                pgList->tailPg = p;
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
            return &i->pg;
    }
    return NULL;
}

