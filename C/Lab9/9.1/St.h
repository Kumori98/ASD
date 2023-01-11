#define MAXLEN 31

typedef struct {
    char nome[MAXLEN];
    int index;
}Item_t;

typedef struct {
    Item_t *items;
    int maxDim, used;
}ST_t;

ST_t STinit(int maxN);
int  STsearch(ST_t ht, char *nome);
void STfree(ST_t ht);
void STinsert(ST_t ht, char *nome, int index);