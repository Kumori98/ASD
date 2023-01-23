#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "titoli.h"

typedef struct nodo_ {
  struct nodo_ *next;
  Titolo t;
} nodo;

typedef nodo* link;

struct LISTtitoli_ {
  link head;
  int count;
};

LISTtitoli LISTtitoliInit() {
  LISTtitoli titoli = calloc(1, sizeof(*titoli));
  return titoli;
}

int LISTtitoliEmpty(LISTtitoli titoli) {
  if (titoli == NULL || titoli->head == NULL || titoli->count == 0)
    return 1;
  return 0;
}

Titolo LISTtitoliSearch(LISTtitoli titoli, char *cod) {
  int cmp;
  link x;
  if (LISTtitoliEmpty(titoli))
    return NULL;
  for(x = titoli->head; x != NULL; x = x->next) {
    cmp = TITOLOcmp((TitoloKey) cod, TITOLOgetKey(x->t));
    if (cmp == 0)
      return x->t;
    if (cmp < 0)
      return NULL;
  }
  return NULL;
}

static link NEW(Titolo t, link next) {
  link l = calloc(1, sizeof(*l));
  l->t = t;
  l->next = next;
  return l;
}

void LISTtitoliInsert(LISTtitoli titoli, Titolo t) {
  link x, p;
  if (titoli == NULL)
    return;
  if (titoli->head == NULL) {
    titoli->head = NEW(t, NULL);
  } else if (TITOLOcmp(TITOLOgetKey(t), TITOLOgetKey(titoli->head->t)) < 0) {
    titoli->head = NEW(t, titoli->head);
  } else {
    for (x=titoli->head->next, p=titoli->head; x!=NULL && TITOLOcmp(TITOLOgetKey(t), TITOLOgetKey(x->t)) > 0; p=x, x=x->next);
    p->next = NEW(t, x);
  }
  titoli->count++;
}

void LISTtitoliStore(LISTtitoli titoli, FILE *fp) {
  link x;
  if (LISTtitoliEmpty(titoli))
    return;
  for (x = titoli->head; x != NULL; x = x->next)
    TITOLOstore(fp, x->t);
}