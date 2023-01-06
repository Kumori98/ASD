#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "titolo.h"

struct TITOLO_ {
  TitoloKey cod;
  float min, max;
  quotazioni_t bstQuotazioni;
};

void TITOLOstore(FILE *fp, Titolo t) {
  fprintf(fp, "%s [MIN = %.2f - MAX = %.2f]\n", (char*) t->cod, t->min, t->max);
  BSTquotazioniStore(t->bstQuotazioni, fp);
}

int TITOLOcmp(TitoloKey t1, TitoloKey t2) {
  return strcmp((char*) t1, (char*) t2);
}

TitoloKey TITOLOgetKey(Titolo t) {
  return t->cod;
}

Titolo TITOLOnew(char *cod) {
  Titolo t = calloc(1, sizeof(*t));
  t->cod = (TitoloKey) strdup(cod);
  t->min = t->max = -1.0;
  t->bstQuotazioni = BSTquotazioniInit();
  return t;
}

static void updateMinmax(Titolo t) {
  float min = -1.0, max = -1.0;
  BSTgetMinMax(t->bstQuotazioni, &min, &max);
  t->min = min;
  t->max = max;
}

void TITOLOinsertTransazione(Titolo t, data_t d, ora_t o, float val, int qta) {
  BSTquotazioniInsert(t->bstQuotazioni, d, val, qta);
  updateMinmax(t);
  return;
}

float TITOLOminAssoluto(Titolo t) {
  if (t == NULL)
    return -1;
  return t->min;
}

float TITOLOmaxAssoluto(Titolo t) {
  if (t == NULL)
    return -1;
  return t->max;
}

static void valRange(Titolo t, data_t d1, data_t d2, float *f1, float *f2) {
  BSTquotazioniMinmaxRange(t->bstQuotazioni, d1, d2, f1, f2);
}

void TITOLOminmaxRange(Titolo t, data_t d1, data_t d2, float *f1, float *f2) {
  int cmp = DATAcmp(d1, d2);
  if (cmp < 0)
    valRange(t, d1, d2, f1, f2);
  else if (cmp > 0)
    valRange(t, d2, d1, f1, f2);
  else
    *f1 = *f2 = QUOTAZIONEgetValore(BSTquotazioniSearch(t->bstQuotazioni, d1));
}

quotazione_t TITOLOgetQuotazione(Titolo t, data_t d) {
  if (t == NULL)
    return initQuotazione();
  if (t->bstQuotazioni == NULL)
    return initQuotazione();
  return BSTquotazioniSearch(t->bstQuotazioni, d);
}

void TITOLOtreeBalance(Titolo t) {
  if (t == NULL || t->bstQuotazioni == NULL)
    return;
  BSTquotazioniBalance(t->bstQuotazioni);
}