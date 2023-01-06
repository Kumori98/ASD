#define MAXL 31

typedef struct Item{
      char elab[MAXL];
      char rete[MAXL];
}Item;

typedef char *Key; //stringa chiave

Key   KEYget(Item *item);
void  KEYscan(char nodo[]);
int   KEYcmp(Key k1, Key k2);
Item  ITEMload(char elab[], char rete[]);
void  ITEMstore(Item item);
Item  ITEMsetNull();

