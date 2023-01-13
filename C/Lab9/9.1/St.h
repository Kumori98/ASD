#define MAXLEN 31

typedef struct Item{
    char nome[MAXLEN];
    int index;
}Item_t;

typedef struct ST{
    Item_t *items;
    int maxDim, used;
}ST_t;

ST_t STinit(int maxN);
int  STsearch(ST_t st, char *nome);
void STfree(ST_t st);
void STinsert(ST_t st, char *nome, int index);
char *STsearchByIndex(ST_t st, int index);