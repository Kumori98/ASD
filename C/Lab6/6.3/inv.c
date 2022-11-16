#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inv.h"

void stat_read(FILE *fp, stat_t *statp){ //fopen fatta in pgList
    fscanf(fp, "%d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia){
    if(soglia){
        fprintf(fp, "HP = %d, MP = %d, ATK = %d, DEF = %d, MAG = %d, SPR = %d\n",
            (statp->hp > 0 ? statp->hp : MIN_STAT), (statp->mp > 0 ? statp->mp : MIN_STAT),
            (statp->atk > 0 ? statp->atk : MIN_STAT), (statp->def > 0 ? statp->def : MIN_STAT),
            (statp->mag > 0 ? statp->mag : MIN_STAT), (statp->spr > 0 ? statp->spr : MIN_STAT));
    }
    else
        fprintf(fp, "HP=%d MP=%d ATK=%d DEF=%d MAG=%d SPR=%d\n", statp->hp, statp->mp, statp->atk, statp->def, statp->mag, statp->spr);
}

void inv_read(FILE *fp, inv_t *invp){
    fscanf(fp, "%s %s", invp->nome, invp->tipo);
    stat_read(fp, &(invp->stat));
}

void inv_print(FILE *fp, inv_t *invp){
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &(invp->stat), 0);
}

stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}