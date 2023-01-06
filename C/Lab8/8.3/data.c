#include "data.h"

int confrontaDate(data_t d1, data_t d2){
    if(d1.anno>d2.anno || (d1.anno==d2.anno && d1.mese>d2.mese) || (d1.anno==d2.anno && d1.mese==d2.mese && d1.giorno>d2.giorno))
        return 1;
    return 0;
}

void stampaData(FILE *fp, data_t d){
    fprintf(fp, "%d/%d/%d", d.giorno, d.mese, d.anno);
}

void stampaOra(FILE *fp, ora_t o){
    fprintf(fp, "%d:%d", o.ore, o.minuti);
}

data_t leggiData(char *s){
  data_t d;
  scanf(s, "%d/%d/%d", &d.anno, &d.mese, &d.giorno);
  return d;    
}

ora_t leggiOra(char *s){
  ora_t o;
  sscanf(s, "%d:%d", &o.ore, &o.minuti);
  return o;
}
