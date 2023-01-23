#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"

Key KEYget(Item *item) {
  return item->elab;
}

void KEYscan(Key nodo) {
  scanf("%s", nodo);
}

int KEYcmp(Key k1, Key k2) {
  return strcmp(k1, k2);
}

Item ITEMload(char elab[], char rete[]) {
  Item x;
  strcpy(x.elab, elab);
  strcpy(x.rete, rete);
  return x;
}

void ITEMstore(Item item) {
  printf("elab: %s net: %s\n", item.elab, item.rete);
}

Item ITEMsetNull() {
  Item val = {" ", " "};
  return val;
}
