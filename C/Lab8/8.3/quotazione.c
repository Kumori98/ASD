#include "quotazione.h"

quotazione_t initQuotazione() {
  quotazione_t q = {{0,0,0}, 0.0, 0};
  return q;
}

int QUOTAZIONEcheckNull(quotazione_t q) {
  return (q.val == 0.0);
}

data_t dataQuotazione(quotazione_t q){
    return q.data;
}

float valQuotazione(quotazione_t q){
    if(q.n>0)
        return q.val / (float) q.n;
    return -1;
}

void stampaQuotazione(FILE *fp, quotazione_t q){
    stampaData(fp, q.data);
    printf(" %.2f\n", valQuotazione(q));
}