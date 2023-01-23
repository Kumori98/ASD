#include<stdlib.h>
#include<stdio.h>

#include "data.h"

typedef struct quotazione {
  data_t data;
  float val;
  int n;
} quotazione_t;

data_t dataQuotazione(quotazione_t q);
float valQuotazione(quotazione_t q);
void stampaQuotazione(FILE *fp, quotazione_t q);
quotazione_t initQuotazione();
int checkNullQuotazione(quotazione_t q);