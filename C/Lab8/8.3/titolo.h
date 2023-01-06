#define LEN 256

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "quotazioni.h"

typedef char* TitoloKey;
typedef struct TITOLO_ *Titolo;

void       TITOLOstore(FILE *out, Titolo t);
int        TITOLOcmp(TitoloKey t1, TitoloKey t2);
TitoloKey  TITOLOgetKey(Titolo t);
Titolo     TITOLOnew(char *cod);
void       TITOLOinsertTransazione(Titolo t, data_t d, ora_t o, float valore, int qta);
float      TITOLOminAssoluto(Titolo t);
float      TITOLOmaxAssoluto(Titolo t);
void       TITOLOminmaxRange(Titolo t, data_t d1, data_t d2, float *f1, float *f2);
quotazione_t TITOLOgetQuotazione(Titolo t, data_t d);
void       TITOLOtreeBalance(Titolo t);