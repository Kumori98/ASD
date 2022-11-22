#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"

typedef struct nodoPg_s *link;

struct nodoPg_s {
  pg_t pg;
  struct nodoPg_s *next;
};

struct pgList_s {
  link head, tail;
  int nPg;
};

link newNode(pg_t pg, link next) {
  link n = malloc(sizeof(*n));
  n->pg = pg;
  n->next = next;
  return n;
}

pgList_t pgList_init() {
  pgList_t pgList = malloc(sizeof(*pgList));
  pgList->head = NULL;
  pgList->nPg = 0;
  return pgList;
}

void pgList_free(pgList_t pgList) {
  link i, next;
  for(i = pgList->head; i != NULL; i=next) {
    next = i->next;
    pg_clean(&i->pg);
    free(i);
  }
}

void pgList_read(FILE *fp, pgList_t pgList) {
  pg_t pg;

  while((pg_read(fp, &pg))!= 0) {
    pgList_insert(pgList, pg);
    pgList->nPg++;
  }

}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
  link i;
  if (pgList == NULL)
    return;
  for(i=pgList->head; i!=NULL; i=i->next)
    pg_print(fp, &i->pg, invArray);
}


void pgList_insert(pgList_t pgList, pg_t pg) {
  link n;
  n = newNode(pg, NULL);
  if (n == NULL || pgList == NULL)
    return;
  if(pgList->head == NULL)
    pgList->head = pgList->tail = n;
  else {
    pgList->tail->next = n;
    pgList->tail = n;
  }
}

void pgList_remove(pgList_t pgList_t, char* cod){
  link i, p;
  for(i = pgList_t->head, p = NULL; i != NULL; p = i, i=i->next) {
    if (strcmp(i->pg.cod, cod) == 0) {
      if (i == pgList_t->head && i == pgList_t->tail) {
        pgList_t->head = pgList_t->tail = NULL;
      } else if (i == pgList_t->head) {
        pgList_t->head = i->next;
      } else if (i == pgList_t->tail) {
        pgList_t->tail = p;
        p->next = NULL;
      } else {
        p->next = i->next;
      }
      pg_clean(&i->pg);
      free(i);
      break;
    }
  }
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
  link i = NULL;
  if (pgList == NULL)
    return NULL;
  for(i = pgList->head; i != NULL; i=i->next) {
    if (strcmp(i->pg.cod, cod) == 0) {
      return (&i->pg);
    }
  }
  return NULL;
}
