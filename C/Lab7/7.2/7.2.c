#include <stdlib.h>
#include <stdio.h>
#define MAXDIAG 2
#define MAXELEM 5
#define MAXLEN 101

typedef struct elemento{
    char nome[MAXLEN];
    int tipologia, direzione_in, direzione_out, precedenza, finale, difficolta;
    float valore;
}elemento_t;

typedef struct diagonale{
    elemento_t elemento[5];
    int difficolta, n, valore;
}diagonale_t;

typedef struct programma{
    diagonale_t diagonali[3];
    int difficolta;
}programma_t;

typedef struct nodo *link; //lista

typedef struct nodo{
    diagonale_t diagonale;
    link next;
}nodo_t;

struct diagonali{ //wrapper lista
    link head;
    int n;
}diagonali;

struct elementi{ 
    elemento_t *elementi;
    int n; 
}elementi;

int caricaElementi(FILE *fp, struct elementi *p_el);
int powerset(int pos, struct elementi *el, int *sol, int cnt, link h, int DD);
link newNode(struct elementi *el, int *sol, link h);
int isvalid(elemento_t el, int DD);


elemento_t last;


int main(){
    struct elementi *p_el;
    int DD, DP,*sol;
    FILE *fp;
    struct diagonali diagonali;

    p_el = (struct elementi*) malloc(sizeof(struct elementi));
    fp = fopen("elementi.txt", "r");
    if(!(caricaElementi(fp, p_el)))
        return -1;

    printf("Inserisci la difficoltà massima per le diagonali e per il programma: ");
    scanf("%d %d", &DD, &DP);

    diagonali.head = malloc(sizeof(nodo_t));
    sol = (int *) malloc(p_el->n*(sizeof(int)));
    last.direzione_out =-1; //sentinella per calcolo powerset
    diagonali.n = powerset(0,p_el,sol,0,diagonali.head, DD);

    printf("%d", diagonali.n);

    return 0;
}

int caricaElementi(FILE *fp, struct elementi *p_el){
    int i;

    if(fp==NULL){
        printf("Errore file");
        fclose(fp);
        return 0;
    }

    fscanf(fp, "%d", &p_el->n);
    p_el->elementi = (elemento_t*) malloc(p_el->n*(sizeof(elemento_t)));
    for(i=0; i<p_el->n; i++)
        fscanf(fp, "%s %d %d %d %d %d %f %d", p_el->elementi[i].nome, &p_el->elementi[i].tipologia, &p_el->elementi[i].direzione_in, &p_el->elementi[i].direzione_out, &p_el->elementi[i].precedenza, &p_el->elementi[i].finale, &p_el->elementi[i].valore, &p_el->elementi[i].difficolta);

    fclose(fp);
    return 1;
}

int acrobatico = 0;
int n_elementi = 0;
int diff_diag = 0;
float val_diag = 0;

int powerset(int pos, struct elementi *el, int *sol, int cnt, link h, int DD){
    if(pos>=el->n){
        if(acrobatico>0){
            h = newNode(el,sol,h); //inserisco in testa
            return cnt+1;            
        }
        return cnt;
    }

    sol[pos]=0;
    cnt = powerset(pos+1,el,sol,cnt,h, DD);
    if(isvalid(el->elementi[pos],DD)){
        sol[pos]=1;
        cnt = powerset(pos+1,el,sol,cnt,h, DD);
        diff_diag -= last.difficolta;
        val_diag -= last.valore;
        if(last.tipologia == 2 || last.tipologia == 1)
            acrobatico--;
        n_elementi--;
    }
    return cnt;
}

link newNode(struct elementi *el, int *sol, link h){
    int i, j=0;
    diagonale_t diagonale;
    link x = malloc(sizeof(*x));
    
    for(i=0; i<el->n; i++){
        if(sol[i]==1){
           diagonale.elemento[j] = el->elementi[i];
           j++;
        }
    }
    diagonale.n = j;
    diagonale.difficolta = diff_diag;
    diagonale.valore = val_diag;
    x->diagonale = diagonale;
    x->next = h;

    return x;
}

int isvalid(elemento_t el, int DD){
    if(last.direzione_out == -1){
        if(el.direzione_in==1 && el.precedenza==0){
            if(el.tipologia==2 || el.tipologia==1)
                acrobatico++;
            last = el;
            diff_diag = el.difficolta;
            val_diag = el.valore;
            n_elementi = 1;
            return 1;
        }
        return 0;
    }

    if((last.direzione_out == el.direzione_in) && (n_elementi+1<5) && (diff_diag+el.difficolta<DD) && (last.finale==0)) {
        if(el.tipologia == 2 || el.tipologia == 1)
            acrobatico++; 
            last = el;
            diff_diag += el.difficolta;
            val_diag += el.valore;
            n_elementi++;
            last.direzione_out = el.direzione_out;
            return 1;
    }
        return 0;

}




