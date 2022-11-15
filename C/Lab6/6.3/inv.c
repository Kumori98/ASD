#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inv.h"

void stat_read(FILE *fp, stat_t *statp){ //fopen fatta in pgList
    fscanf(fp, "%d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia){
    if(soglia){
        if(statp->hp<0)
            fprintf(fp, "HP=0 ");
        else
            fprintf(fp, "HP=%d ", statp->hp);
        if(statp->mp<0)
            fprintf(fp, "MP=0 ");
        else
            fprintf(fp, "MP=%d ", statp->mp);
        if(statp->atk<0)
            fprintf(fp, "ATK=0 ");
        else
            fprintf(fp, "ATK=%d ", statp->atk);
        if(statp->def<0)
            fprintf(fp, "DEF=0 ");
        else
            fprintf(fp, "DEF=%d ", statp->def);
        if(statp->mag<0)
            fprintf(fp, "MAG=0 ");
        else
            fprintf(fp, "MAG=%d ", statp->mag);
        if(statp->spr<0)
            fprintf(fp, "SPR=0 ");
        else
            fprintf(fp, "SPR=%d ", statp->spr);
    }
    else
        fprintf(fp, "HP=%d MP=%d ATK=%d DEF=%d MAG=%d SPR=%d", statp->hp, statp->mp, statp->atk, statp->def, statp->mag, statp->spr);
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