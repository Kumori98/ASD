#include <stdlib.h>
#include <stdio.h>
#define MAXDIAG 3
#define MAXELEM 5
#define MAXLEN 101

typedef struct elemento{
    char nome[MAXLEN];
    int tipologia, direzione_in, direzione_out, precedenza, finale, difficolta, seq;
    float valore;
}elemento_t;

typedef struct diagonale{
    elemento_t elemento[MAXELEM];
    int difficolta, n;
    float valore;
    int acrobatico_av;
    int acrobatico_ind;
    int acrobatico_seq;
}diagonale_t;

typedef struct programma{
    diagonale_t diagonali[MAXDIAG];
    int difficolta, n;
    float valore;
}programma_t;

struct diagonali{ //wrapper lista
    diagonale_t *diagonali;
    int n;
}diagonali;

struct elementi{ 
    elemento_t *elementi;
    int n; 
}elementi;

diagonale_t last_d;
diagonale_t sample;
elemento_t last;
int acrobatico_av=0;
int acrobatico_ind=0;
int acrobatico_seq=0;
int size = 14000; //per allocare il vettore di diagonali

int caricaElementi(FILE *fp, struct elementi *p_el);
int disp_rip_diag(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k);
int isvalid(elemento_t el, int DD, int pos, int k,  diagonale_t *diagonale);
void riempiDiag(struct elementi *el, int *sol, int k, diagonale_t *diagonale);
int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD);
float calcolaProgramma(programma_t *programma, struct diagonali diagonali, int DP);
void disp_rip_progr(int pos, struct diagonali diag, int *sol, programma_t *programma, int DP, int k);


int main(){
    struct elementi *p_el;
    int DD=10, DP=20;
    FILE *fp;
    struct diagonali diagonali;
    int i, j;
    programma_t programma;

    diagonali.n = 0;
    p_el = (struct elementi*) malloc(sizeof(struct elementi));
    fp = fopen("elementi.txt", "r");
    if(!(caricaElementi(fp, p_el)))
        return -1;

    //printf("Inserisci la difficoltà massima per le diagonali e per il programma: ");
    //scanf("%d %d", &DD, &DP);

    diagonali.diagonali = (diagonale_t*) malloc((size)*sizeof(diagonale_t)); //eventualmente rialloco
    diagonali.n = calcoloDiagonali(p_el,&diagonali,DD);

    programma.valore = calcolaProgramma(&programma, diagonali, DP);
    printf("-- Test con DD= %d e DP=%D --\n Totale: %d", DD, DP, programma.valore);
    for(i=0; i<programma.n; i++){
        printf("Diagonale #%d > %f\n", i, programma.diagonali[i].valore);
        for(j=0; j<programma.diagonali[i].n; j++)
            printf("%s ", programma.diagonali[i].elemento[j].nome);
                printf("\n");
    }

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

int disp_rip_diag(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k){
    int i, j;
    if(pos>=k){
        if(diagonali->diagonali[cnt].acrobatico_av>0 || diagonali->diagonali[cnt].acrobatico_ind>0){
            if(cnt+5>=size){
                size = 2*size;
                diagonali->diagonali = realloc(diagonali->diagonali,size*sizeof(diagonale_t));
                for(j=(size/2)-1; j<size; j++) //setto a default tutte le nuove diagonali allocate
                    diagonali->diagonali[j] = sample;
            }
            riempiDiag(el,sol,k,&(diagonali->diagonali[cnt])); 
            cnt++;         
        }
        return cnt;
    }
    for(i=0; i<el->n; i++){
         if(isvalid(el->elementi[i], DD, pos, k, &(diagonali->diagonali[cnt]))){
            sol[pos]=i;
            cnt = disp_rip_diag(pos+1, el, sol, cnt, diagonali, DD, k);
            if(diagonali->diagonali[cnt].difficolta!=0){ //se sto facendo backtrack
                diagonali->diagonali[cnt].difficolta -= last.difficolta;
                diagonali->diagonali[cnt].valore -= last.valore;
                if(last.tipologia == 2)
                    diagonali->diagonali[cnt].acrobatico_av--;
                if(last.tipologia == 1)
                    diagonali->diagonali[cnt].acrobatico_ind--;
                if(last.seq==1)
                    diagonali->diagonali[cnt].acrobatico_seq--;
            }
            if(pos!=0)
                last = el->elementi[sol[pos-1]];
            else{
                last.direzione_out = -1;
                last.tipologia = -1;
            }
         }
    }
    return cnt;
}

void riempiDiag(struct elementi *el, int *sol, int k, diagonale_t *diagonale){
    int i;
    
    for(i=0; i<k; i++)
        diagonale->elemento[i] = el->elementi[sol[i]];
    diagonale->n = k;
    return;
}

int isvalid(elemento_t el, int DD, int pos, int k, diagonale_t *diagonale){
    if(pos==0){ //primo elemento della diagonale
        if(el.direzione_in==1 && el.precedenza==0 && el.difficolta<DD){
            if(el.tipologia == 2){
                diagonale->acrobatico_av++;
                if(last.tipologia == 1 || last.tipologia == 2){
                    el.seq=1;
                    diagonale->acrobatico_seq++;
                }
            }
            else if(el.tipologia == 1){
                diagonale->acrobatico_ind++;
                if(last.tipologia == 1 || last.tipologia == 2){
                    el.seq=1;
                    diagonale->acrobatico_seq++;
                }
            }
            last = el;
            diagonale->difficolta += el.difficolta;
            diagonale->valore += el.valore;
            return 1;
        }
        return 0;
    }

    if(pos+1==k){ //ultimo elemento
        if((last.direzione_out == el.direzione_in) && (diagonale->difficolta+el.difficolta<=DD)) {
            if(el.tipologia == 2){
                diagonale->acrobatico_av++;
                if(last.tipologia == 1 || last.tipologia == 2){
                    el.seq=1;
                    diagonale->acrobatico_seq++;
                    }
                }
            else if(el.tipologia == 1){
                    diagonale->acrobatico_ind++;
                    if(last.tipologia == 1 || last.tipologia == 2){
                        el.seq=1;
                        diagonale->acrobatico_seq++;
                    }
                }  
            last = el;
            diagonale->difficolta += el.difficolta;
            diagonale->valore += el.valore;
            return 1;
        }
        return 0;

    }

    if((last.direzione_out == el.direzione_in) && (diagonale->difficolta+el.difficolta<=DD) && (el.finale==0)){
        if(el.tipologia == 2){
                diagonale->acrobatico_av++;
                if(last.tipologia == 1 || last.tipologia == 2){
                    el.seq=1;
                    diagonale->acrobatico_seq++;
                    }
        else if(el.tipologia == 1){
                    diagonale->acrobatico_ind++;
                    if(last.tipologia == 1 || last.tipologia == 2){
                        el.seq=1;
                    diagonale->acrobatico_seq++;
                }  
            last = el;
            diagonale->difficolta += el.difficolta;
            diagonale->valore += el.valore;
            return 1;
            }
        return 0;
        }
    }
}


int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD){
    int *sol, k;

    sol = (int *) malloc(MAXELEM*sizeof(int));

    for(k=0; k<MAXELEM; k++)
        sol[k]=-1;

    sample.acrobatico_av=0;
    sample.acrobatico_ind=0;
    sample.acrobatico_seq=0;
    sample.difficolta=0;
    sample.valore=0;

    for(k=0; k<size; k++)
        diagonali->diagonali[k] = sample;

    for(k=1; k<6; k++){
        diagonali->n += disp_rip_diag(0,p_el,sol,diagonali->n,diagonali, DD, k);
        last.direzione_out = -1;
        last.tipologia = -1;
    }
    diagonali->diagonali = realloc(diagonali->diagonali,diagonali->n*sizeof(diagonale_t));
    free(sol);
    return diagonali->n;
}


float calcolaProgramma(programma_t *programma, struct diagonali diagonali, int DP){
    int k, *sol;

    sol = (int *) malloc(MAXDIAG*sizeof(int));

    for(k=0; k<MAXDIAG; k++)
        sol[k]=-1;

    programma->valore = 0;

    for(k=1; k<4; k++){
        disp_rip_progr(0,diagonali,sol,programma,DP,k);
    }
    free(sol);
    return programma->n;
}


void disp_rip_progr(int pos, struct diagonali diag, int *sol, programma_t *programma, int DP, int k){
    int i, j;
    float val;
    if(pos==k){
        if(acrobatico_av>0 && acrobatico_ind>0 && acrobatico_seq>0){
           for(j=0; j<k; j++)
            val += diag.diagonali[sol[j]].valore;
            if(val>programma->valore){
                programma->valore = val;
                programma->n = k;
                for(j=0; j>k; j++)
                    programma->diagonali[j] = diag.diagonali[sol[j]];
            }           
        }
        return;
    }
    for(i=0; i<diag.n; i++){
        if(programma->difficolta + diag.diagonali[sol[pos]].difficolta <= DP){
            sol[pos]=i;
            disp_rip_progr(pos+1, diag, sol, programma, DP, k);
            if(last_d.acrobatico_av>0)
                acrobatico_av -= last_d.acrobatico_av;
            if(last_d.acrobatico_ind>0)
                acrobatico_ind -= last_d.acrobatico_ind;
            if(last_d.acrobatico_seq>0)
                acrobatico_seq -= last_d.acrobatico_seq;
            if(pos>0)
                last_d = diag.diagonali[sol[pos-1]];
         }
    }
    return;
}







