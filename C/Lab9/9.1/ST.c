#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ST.h"

ST_t STinit(int maxN){
    ST_t st;
    int i;
    st.items = malloc(maxN*sizeof(Item_t));
    st.maxDim = maxN;
    st.used = 0;
    for (i = 0; i < st.maxDim; i++)
        st.items[i].index = -1;
    return st;
}

int  STsearch(ST_t st, char *nome){
    int i;
    for(i=0; i<st.used; i++)
        if(strcmp(st.items[i].nome,nome)==0)
            return st.items[i].index;
    return -1;
    
}

void STfree(ST_t st){
    if(st.items==NULL)
        return;
    free(st.items);
}

void STinsert(ST_t st, char *nome, int index){
    if(st.items==NULL)
        return;
    if(st.used+1 > st.maxDim){
        st.items = realloc(st.items, 2*st.maxDim*sizeof(Item_t));
        st.maxDim = 2*st.maxDim;
    }
    st.used++;
    strcpy(st.items[st.used].nome,nome);
    st.items[st.used].index = st.used;
}