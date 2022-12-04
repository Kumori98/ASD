#include <stdlib.h>
#include <stdio.h>
#define MAXDIAG 3
#define MAXELEM 5
#define MAXLEN 101

typedef struct elemento{
    char nome[MAXLEN];
    int tipologia, direzione_in, direzione_out, precedenza, finale, difficolta;
    float valore;
}elemento_t;

typedef struct diagonale{
    elemento_t elemento[5];
    int difficolta, n;
    float valore;
}diagonale_t;

typedef struct programma{
    diagonale_t diagonali[3];
    int difficolta;
}programma_t;

struct diagonali{ //wrapper lista
    diagonale_t *diagonali;
    int n;
}diagonali;

struct elementi{ 
    elemento_t *elementi;
    int n; 
}elementi;

int caricaElementi(FILE *fp, struct elementi *p_el);
int disp_rip(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k, int size);
int isvalid(elemento_t el, int DD, int pos, int k);
diagonale_t newDiag(struct elementi *el, int *sol, int k);
int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD, int size);


int main(){
    struct elementi *p_el;
    int DD=10, DP=20;
    FILE *fp;
    struct diagonali diagonali;
    int size = 2000;

    diagonali.n = 0;
    p_el = (struct elementi*) malloc(sizeof(struct elementi));
    fp = fopen("elementi.txt", "r");
    if(!(caricaElementi(fp, p_el)))
        return -1;

    //printf("Inserisci la difficoltà massima per le diagonali e per il programma: ");
    //scanf("%d %d", &DD, &DP);

    diagonali.diagonali = (diagonale_t*) malloc((size)*sizeof(diagonale_t)); //eventualmente rialloco
    diagonali.n = calcoloDiagonali(p_el,&diagonali,DD, size);

    printf("%d", diagonali.n);

    free(p_el->elementi);
    free(p_el);
    free(diagonali.diagonali);
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
int diff_diag = 0;
float val_diag = 0;
elemento_t last;

int disp_rip(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k, int size){
    int i;
    if(pos==k){
        if(acrobatico>0){
            if(cnt>=size){
                size = 2*size;
                diagonali->diagonali = realloc(diagonali->diagonali,size*sizeof(diagonale_t));
            }
            diagonali->diagonali[cnt] = newDiag(el,sol,k); 
            cnt++;         
        }
        return cnt;
    }
    for(i=0; i<el->n; i++){
         if(isvalid(el->elementi[i],DD, pos, k)){
            sol[pos]=i;
            cnt = disp_rip(pos+1, el, sol, cnt, diagonali, DD, k, size);
            diff_diag -= last.difficolta;
            val_diag -= last.valore;
            if(last.tipologia == 2 || last.tipologia == 1)
                acrobatico--;
            if(pos>0)
                last = el->elementi[sol[pos-1]];
         }
    }
    return cnt;
}

diagonale_t newDiag(struct elementi *el, int *sol, int k){
    int i;
    diagonale_t diagonale;
    
    for(i=0; i<k; i++)
        diagonale.elemento[i] = el->elementi[sol[i]];
    diagonale.n = k;
    diagonale.difficolta = diff_diag;
    diagonale.valore = val_diag;

    return diagonale;
}

int isvalid(elemento_t el, int DD, int pos, int k){
    if(pos==0){ //primo elemento della diagonale
        if(el.direzione_in==1 && el.precedenza==0){
            if(el.tipologia==2 || el.tipologia==1)
                acrobatico++;
            last = el;
            diff_diag = el.difficolta;
            val_diag = el.valore;
            return 1;
        }
        return 0;
    }

    if(pos+1==k){
        if((last.direzione_out == el.direzione_in) && (diff_diag+el.difficolta<=DD)) {
        if(el.tipologia == 2 || el.tipologia == 1)
            acrobatico++; 
            last = el;
            diff_diag += el.difficolta;
            val_diag += el.valore;
            last.direzione_out = el.direzione_out;
            return 1;
    }
        return 0;

    }

    if((last.direzione_out == el.direzione_in) && (diff_diag+el.difficolta<=DD) && (el.finale==0)){
        if(el.tipologia == 2 || el.tipologia == 1)
            acrobatico++; 
            last = el;
            diff_diag += el.difficolta;
            val_diag += el.valore;
            last.direzione_out = el.direzione_out;
            return 1;
    }
        return 0;

}

int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD, int size){
    int *sol, k;

    sol = (int *) malloc(MAXELEM*sizeof(int));

    for(k=0; k<MAXELEM; k++)
        sol[k]=-1;

    for(k=5; k>=1; k--){
        diagonali->n += disp_rip(0,p_el,sol,diagonali->n,diagonali, DD, k, size);
        acrobatico = 0;
        diff_diag = 0;
        val_diag = 0;
        last.direzione_out = -1;
    }
    diagonali->diagonali = realloc(diagonali->diagonali,diagonali->n*sizeof(diagonale_t));
    free(sol);
    return diagonali->n;
}




