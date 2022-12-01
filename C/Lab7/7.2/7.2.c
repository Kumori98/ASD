#include <stdlib.h>
#include <stdio.h>
#define NOMEFILE "elementi.txt"
#define MAXLEN 101

typedef struct elemento{
    char nome[MAXLEN];
    int tipologia, direzione_in, direzione_out, precedenza, finale, difficolta;
    float valore;
}elemento_t;

typedef struct diagonale{
    elemento_t *elemento;
    int difficolta, n;
}diagonale_t;

typedef struct programma{
    diagonale_t diagonali[3];
    int difficolta;
}programma_t;

struct diagonali{
    diagonale_t *diagonali;
    int n;
}diagonali;

struct elementi{
    elemento_t *elementi;
    int n;
}elementi;

int caricaElementi(FILE *fp, struct elementi *p_el);


int main(){
    struct elementi *p_el;
    FILE *fp;

    if(!(caricaElementi(fp, p_el)))
        return -1;
    
    printf("%d", p_el->n);

    

    return 0;
}

int caricaElementi(FILE *fp, struct elementi *p_el){
    fp = fopen(NOMEFILE, "r");
    int i;

    p_el = (struct elementi*) malloc(sizeof(struct elementi));

    if(fp==NULL){
        printf("Errore");
        return 0;
    }

    fscanf(fp, "%d", &p_el->n);
    p_el->elementi = (elemento_t*) malloc(p_el->n*(sizeof(elemento_t)));
    for(i=0; i<p_el->n; i++)
        fscanf(fp, "%s %d %d %d %d %d %f %d", p_el->elementi[i].nome, &p_el->elementi[i].tipologia, &p_el->elementi[i].direzione_in, &p_el->elementi[i].direzione_out, &p_el->elementi[i].precedenza, &p_el->elementi[i].finale, &p_el->elementi[i].valore, &p_el->elementi[i].difficolta);
    return 1;
}


