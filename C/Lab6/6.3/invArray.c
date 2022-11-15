#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "invArray.h"
#define NOMEFILEOUT "out.txt"

struct invArray_s{
    inv_t *vettInv;
    int nInv;
    int maxInv;
};

invArray_t invArray_init(){
    invArray_t invArray;
    invArray = malloc(sizeof(*invArray));
    invArray->nInv = 0;
    return invArray;
}

void invArray_free(invArray_t invArray){
    free(invArray->vettInv);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray){
    int i;
    
    fscanf(fp, "%d", &invArray->nInv);
    invArray->vettInv = malloc(invArray->nInv*sizeof(inv_t));
    for(i=0; i<invArray->nInv; i++){
        inv_read(fp, &invArray->vettInv[i]);
    }
    fclose(fp);
}

void invArray_print(FILE *fp, invArray_t invArray){
    int i;

    fp = fopen(NOMEFILEOUT, "w");
    if(fp==NULL)
        return;
    for(i=0; i<invArray->nInv; i++){
        inv_print(fp, &invArray->vettInv[i]);
    }
    fclose(fp);
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    fp = fopen(NOMEFILEOUT, "w");
    if(fp==NULL)
        return;
    inv_print(fp, &invArray->vettInv[index]);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &invArray->vettInv[index];
}

int invArray_searchByName(invArray_t invArray, char *name){
    int i;

    for(i=0; i<invArray->nInv; i++){
        if(strcmp(invArray->vettInv[i].nome,name)==0)
            return i;
    }
    return -1;
}