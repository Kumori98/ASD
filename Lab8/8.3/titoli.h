#define LEN 256
#include<stdlib.h>
#include<stdio.h>
#include "titolo.h"

typedef struct LISTtitoli_ *LISTtitoli;

int        LISTtitoliEmpty(LISTtitoli titoli);
Titolo     LISTtitoliSearch(LISTtitoli titoli, char *cod);
void       LISTtitoliInsert(LISTtitoli titoli, Titolo t);
LISTtitoli LISTtitoliInit();
void       LISTtitoliStore(LISTtitoli titoli, FILE *fp);
