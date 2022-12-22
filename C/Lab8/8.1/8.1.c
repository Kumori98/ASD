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
    int bonus;
}diagonale_t;

typedef struct programma{
    diagonale_t diagonali[MAXDIAG];
    int difficolta;
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

int diff_diag=0;
int diff_prog=0;
int acrobatico_av=0;
int acrobatico_ind=0;
int acrobatico_seq=0;
int size = 1000; //per allocare il vettore di diagonali

int caricaElementi(FILE *fp, struct elementi *p_el);
int disp_rip_diag(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k);
int isvalid(elemento_t *el, int DD, int pos, int k,  int i, int last);
diagonale_t newDiag(struct elementi *el, int *sol, int k);
int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD);
programma_t calcolaProgramma_greedy(struct diagonali diagonali, int DP);
diagonale_t* bubbleSort(diagonale_t *diagonali, int N);



int main(){
    struct elementi *p_el;
    int DD, DP;
    FILE *fp;
    struct diagonali diagonali;
    int i, j;
    programma_t programma;

    diagonali.n = 0;
    p_el = (struct elementi*) malloc(sizeof(struct elementi));
    fp = fopen("elementi.txt", "r");
    if(!(caricaElementi(fp, p_el)))
        return -1;

    printf("Inserisci la difficoltà massima per le diagonali e per il programma: ");
    scanf("%d %d", &DD, &DP);

    diagonali.diagonali = (diagonale_t*) malloc((size)*sizeof(diagonale_t)); //eventualmente rialloco
    diagonali.n = calcoloDiagonali(p_el,&diagonali,DD);

    programma = calcolaProgramma_greedy(diagonali, DP);
    printf("-- Test con DD= %d e DP=%d --\n Totale: %.3f\n", DD, DP, programma.valore);
    for(i=0; i<MAXDIAG; i++){
        if(i==MAXDIAG-1 && programma.diagonali[i].bonus==1)
            printf("Diagonale #%d > %.3f * 1.5 (bonus)\n", i, programma.diagonali[i].valore);
        else
            printf("Diagonale #%d > %.3f\n", i, programma.diagonali[i].valore);
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
    for(i=0; i<p_el->n; i++){
         fscanf(fp, "%s %d %d %d %d %d %f %d", p_el->elementi[i].nome, &p_el->elementi[i].tipologia, &p_el->elementi[i].direzione_in, &p_el->elementi[i].direzione_out, &p_el->elementi[i].precedenza, &p_el->elementi[i].finale, &p_el->elementi[i].valore, &p_el->elementi[i].difficolta);
         p_el->elementi[i].seq = 0;
    }

    fclose(fp);
    return 1;
}

int disp_rip_diag(int pos, struct elementi *el, int *sol, int cnt, struct diagonali *diagonali, int DD, int k){
    int i, j;
    if(pos>=k){
        if(acrobatico_av>0 || acrobatico_ind>0){
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
         if(isvalid(el->elementi, DD, pos, k, i, sol[pos-1])){
            sol[pos]=i;
            cnt = disp_rip_diag(pos+1, el, sol, cnt, diagonali, DD, k);
            diff_diag -= el->elementi[i].difficolta;
            if(el->elementi[i].tipologia==2)
                acrobatico_av--;
            if(el->elementi[i].tipologia==1)
                acrobatico_ind--;
            if(el->elementi[i].seq==1){
                acrobatico_seq--;
                el->elementi[i].seq=0;
            }
        }
    }
    return cnt;
}

diagonale_t newDiag(struct elementi *el, int *sol, int k){
    int i;
    diagonale_t diagonale;
    diagonale.valore=0;
    diagonale.difficolta=0;
    
    for(i=0; i<k; i++){
        diagonale.elemento[i] = el->elementi[sol[i]];
        diagonale.difficolta += el->elementi[sol[i]].difficolta;
        diagonale.valore += el->elementi[sol[i]].valore; 
        if(i==k-1)
            if(el->elementi[sol[i]].difficolta>=8)
                diagonale.bonus=1;
            else
                diagonale.bonus=0;
    }
    diagonale.acrobatico_av = acrobatico_av;
    diagonale.acrobatico_ind = acrobatico_ind;
    diagonale.acrobatico_seq = acrobatico_seq;
    diagonale.n = k;
    return diagonale;
}

int isvalid(elemento_t *el, int DD, int pos, int k, int i, int last){

    if(pos==0){ //primo elemento della diagonale
        if(el[i].direzione_in==1 && el[i].precedenza==0 && el[i].difficolta<DD){
            if(el[i].tipologia == 2)
                acrobatico_av++;
            if(el[i].tipologia == 1)
                acrobatico_ind++;
            diff_diag += el[i].difficolta;
            return 1;
        }
        return 0;
    }

    if(pos+1==k){ //ultimo elemento
        if((el[last].direzione_out == el[i].direzione_in) && (diff_diag+el[i].difficolta<=DD)) {
            if(el[i].tipologia == 2){
                acrobatico_av++;
                if(el[last].tipologia == 1 || el[last].tipologia == 2){
                    el[i].seq=1;
                    acrobatico_seq++;
                    }
                }
            if(el[i].tipologia == 1){
                acrobatico_ind++;
                    if(el[last].tipologia == 1 || el[last].tipologia == 2){
                        el[i].seq=1;
                        acrobatico_seq++;
                    }
                }  
            diff_diag += el[i].difficolta;
            return 1;
        }
        return 0;
    }


    if((el[last].direzione_out == el[i].direzione_in) && (diff_diag+el[i].difficolta<=DD) && (el[i].finale==0)){
        if(el[i].tipologia == 2){
                acrobatico_av++;
                if(el[last].tipologia == 1 || el[last].tipologia == 2){
                    el[i].seq=1;
                    acrobatico_seq++;
                    }
            }

        if(el[i].tipologia == 1){
                acrobatico_ind++;
                if(el[last].tipologia == 1 || el[last].tipologia == 2){
                    el[i].seq=1;
                    acrobatico_seq++;
                }
            }  
        diff_diag += el[i].difficolta;
        return 1;
        }
    return 0;  
}


int calcoloDiagonali(struct elementi *p_el, struct diagonali *diagonali, int DD){

    int *sol, k;

    sol = (int *) malloc(MAXELEM*sizeof(int));
    
    diagonali->n = 0;

    for(k=1; k<MAXELEM+1; k++){
        diagonali->n += disp_rip_diag(0,p_el,sol,diagonali->n,diagonali, DD, k);
        diff_diag = 0;
        acrobatico_av=0;
        acrobatico_ind=0;
        acrobatico_seq=0;
    }
    
    diagonali->diagonali = realloc(diagonali->diagonali,diagonali->n*sizeof(diagonale_t));
    free(sol);
    return diagonali->n;
}

programma_t calcolaProgramma_greedy(struct diagonali diagonali, int DP){
    programma_t programma;
    int i, j;

    diagonali.diagonali = bubbleSort(diagonali.diagonali, diagonali.n);
    programma.valore=0;

    for(i=0; i<MAXDIAG; i++){
        for(j=0; j<diagonali.n; j++){
            if(diff_prog+diagonali.diagonali[j].difficolta<DP){
                if(i==MAXDIAG-1 && acrobatico_av<0 && diagonali.diagonali[j].acrobatico_av==0)
                    continue;
                if(i==MAXDIAG-1 && acrobatico_ind<0 && diagonali.diagonali[j].acrobatico_ind==0)
                    continue;
                if(i==MAXDIAG-1 && acrobatico_seq<0 && diagonali.diagonali[j].acrobatico_seq==0)
                    continue;
                programma.diagonali[i] = diagonali.diagonali[j];
                programma.valore += diagonali.diagonali[j].valore;
                diff_prog += diagonali.diagonali[j].difficolta;
                if(diagonali.diagonali[j].acrobatico_av>0)
                    acrobatico_av++;
                if(diagonali.diagonali[j].acrobatico_ind>0)
                    acrobatico_ind++;
                if(diagonali.diagonali[j].acrobatico_seq>0)
                    acrobatico_seq++;
                break;
            }
        }
    }
    return programma;
}

diagonale_t* bubbleSort(diagonale_t *diagonali, int N){
    int i, j, l=0, r=N-1;
    diagonale_t temp;

    for(i=l; i<r; i++){
        for(j=l; j<r-i-l; j++){
            if(diagonali[j].valore<diagonali[j+1].valore){
                temp = diagonali[j];
                diagonali[j] = diagonali[j+1];
                diagonali[j+1] = temp;
            } 
        }
    }
    return diagonali;
}