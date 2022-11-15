#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "equipArray.h"

struct equipArray_s{
    int vettEq[EQUIP_SLOT];
    int inUso;
};

equipArray_t equipArray_init(){
    int i;
    equipArray_t equipArray;
    equipArray = malloc(sizeof(*equipArray));
    for(i=0; i<EQUIP_SLOT; i++)
        equipArray->vettEq[i] = -1;
    equipArray->inUso = 0;
    return equipArray;
}

void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray){
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    int i;
    for(i=0; i<EQUIP_SLOT; i++){
        if(equipArray->vettEq[i]!=-1)
            invArray_printByIndex(fp, invArray, equipArray->vettEq[i]);
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray){
    int scelta, eq, i;
    char nome[LEN];
    
    printf("Cosa si vuole fare (Si inserisca l'intero relativo all'opzione)?\n");
    if(equipArray->inUso<EQUIP_SLOT)
        printf("1) Aggiungere;\n");
    if(equipArray->inUso>0)
        printf("2) Rimuovere;\n");
    scanf("%d", &scelta);

    printf("Inserisca il nome dell'oggetto: ");
    scanf("%s", nome);
    eq = invArray_searchByName(invArray, nome);

    if(scelta == 1){
        for(i=0; i<EQUIP_SLOT; i++){
            if(equipArray->vettEq[i] == -1){
                equipArray->vettEq[i] = eq;
                equipArray->inUso++;
                printf("Oggetto aggiunto!\n");
                break;
            }
        }
        printf("Oggetto non trovato!\n");
    }
    else if(scelta == 2){
         for(i=0; i<EQUIP_SLOT; i++){
            if(equipArray->vettEq[i] == eq){
                equipArray->vettEq[i] = -1;
                equipArray->inUso--;
                printf("Oggetto rimosso!\n");
                break;
            }
        }
        printf("Oggetto non trovato!\n");
    }
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    return equipArray->vettEq[index];
}