#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp){
    if(fscanf(fp,"%s %s %s", pgp->cod, pgp->nome, pgp->classe)==3){
        stat_read(fp, &(pgp->b_stat));
        pgp->eq_stat = pgp->b_stat; 
        pgp->equip = equipArray_init();
        return 1; 
    }  
    return 0;
}

void pg_clean(pg_t *pgp){
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){
    fprintf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->b_stat, 1); //devo fare il check della soglia
    if(equipArray_inUse(pgp->equip)>0){
        fprintf(fp, " Con eq: ");
        stat_print(fp, &pgp->eq_stat, 1);
        equipArray_print(fp, pgp->equip, invArray);
    }
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    int i, eq;
    inv_t *inv;

    equipArray_update(pgp->equip, invArray);

    pgp->eq_stat = pgp->b_stat; //resetto le statistiche
    for(i=0;i<EQUIP_SLOT;i++) {
        eq = equipArray_getEquipByIndex(pgp->equip, i);
        if (eq == -1)
            continue;
        inv = invArray_getByIndex(invArray, eq);
        pgp->eq_stat.hp = (pgp->eq_stat.hp + inv_getStat(inv).hp);
        pgp->eq_stat.mp = (pgp->eq_stat.mp + inv_getStat(inv).mp);
        pgp->eq_stat.atk = (pgp->eq_stat.atk + inv_getStat(inv).atk);
        pgp->eq_stat.def = (pgp->eq_stat.def + inv_getStat(inv).def);
        pgp->eq_stat.mag = (pgp->eq_stat.mag + inv_getStat(inv).mag);
        pgp->eq_stat.spr = (pgp->eq_stat.spr + inv_getStat(inv).spr);
  }
}