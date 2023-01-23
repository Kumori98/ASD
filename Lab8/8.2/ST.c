#include "Item.h"
#include "ST.h"
#include <stdio.h>
#include <stdlib.h>

struct ST {
    Item *item;
    int maxN, size;
    };

ST STinit(int maxN){
    int i;
    ST st = malloc(sizeof(*st));

    st->item = malloc(maxN*sizeof(Item));
    for (i = 0; i < maxN; i++)
        st->item[i] = ITEMsetNull();
    st->maxN = maxN;
    st->size = 0;
  return st;
}

void STinsert(ST st, Item item){
    int i;
    if (st->size > st->maxN) {
        st->item = realloc(st->item, (2*st->maxN)*sizeof(Item));
        st->maxN = 2*st->maxN;
    }

    for(i = st->size+1; i>0 && KEYcmp(KEYget(&item),KEYget(&st->item[i-1]))==-1; i--){
        st->item[i] = st->item[i-1];
    }
    st->item[i] = item;
}

void STfree(ST st) {
  free(st->item);
  free(st);
}

int STcount(ST st) {
  return st->size;
}

int searchR(ST st, int l, int r, Key k) {
  int m = (l + r)/2; 
  if (l > r)
    return -1;
  if (KEYcmp(k, KEYget(&(st->item[m])))==0)
    return m;
  if (l == r)
    return -1;
  if (KEYcmp(k, KEYget(&(st->item[m])))==-1)
    return searchR(st, l, m-1, k);
  else
    return searchR(st, m+1, r, k);
}

int STsearch(ST st, Key k) {
    return searchR(st, 0, st->size-1, k) ;
}


