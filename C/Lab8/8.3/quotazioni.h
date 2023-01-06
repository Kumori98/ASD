#include<stdlib.h>
#include<stdio.h>

#include "data.h"
#include "quotazione.h"

#define S 2.0

typedef struct quotazioni *quotazioni_t;

int           BSTquotazioniEmpty(quotazioni_t bst);
quotazione_t  BSTquotazioniSearch(quotazioni_t bst, data_t d);
void          BSTquotazioniInsert(quotazioni_t bst, data_t d, float val, int n);
void          BSTquotazioniMinmaxRange(quotazioni_t bst, data_t d1, data_t d2, float *f1, float *f2);
quotazioni_t  BSTquotazioniInit();
void          BSTquotazioniStore(quotazioni_t bst, FILE *fp);
void          BSTquotazioniBalance(quotazioni_t bst);
void          BSTgetMinMax(quotazioni_t bst, float *min, float *max);