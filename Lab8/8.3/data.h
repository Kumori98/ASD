#define DATELEN 13 
#define TIMELEN 6  
#include<stdlib.h>
#include<stdio.h>


typedef struct data{
  int anno, mese, giorno;
} data_t;

typedef struct ora{
  int ore, minuti;
} ora_t;

int confrontaDate(data_t d1, data_t d2);
void stampaData(FILE *fp, data_t d);
void stampaOra(FILE *fp, ora_t o);
data_t leggiData(char *s);
ora_t leggiOra(char *s);
