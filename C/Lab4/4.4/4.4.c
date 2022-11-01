#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "test_set.txt"

int ricerca_max(int *pietre, int *scelte, int pietre_distinte, int tot, int *sol_max, int *prezzi, int maxrip, int *len);
int is_valid(int *pietre, int *sol, int pos, int k, int maxrip, int *prezzi);
int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k, int *prezzi, int maxrip, int *sol_k);
int check_rip(int *sol, int pos, int maxrip);
int calcola_prezzo(int *collana, int k, int *prezzi);

int n_smer, n_zaff, prezzo_k;

int main(){
    FILE *fp;
    int n_test, i, pietre[4], tot, len, *sol, j, pietre_dist=0, *scelte, prezzi[4], maxrip, prezzo_max;



    fp = fopen(NOMEFILE, "r");
    if(fp==NULL){
        printf("File non trovato");
        return -1;
    }

    fscanf(fp, "%d", &n_test);

    for(i=0; i<n_test; i++){
        pietre_dist=0;
        tot=0;
        prezzo_k=0;
        for(j=0; j<9; j++){
            if(j<4){
                fscanf(fp, "%d", &pietre[j]);
                if(pietre[j]!=0)
                    pietre_dist++; 
                tot += pietre[j];
            }
            else if(j<8)
                fscanf(fp, "%d", &prezzi[j-4]);
            else
                fscanf(fp, "%d", &maxrip);
        }
        scelte = (int *) malloc(pietre_dist*sizeof(int));
        for(j=0; j<pietre_dist;j++)
            scelte[j]=j;
        sol = (int *) malloc(tot*sizeof(int)); //soluzione massima di dimensione tot
        prezzo_max = ricerca_max(pietre, scelte, pietre_dist, tot, sol, prezzi, maxrip, &len);
        printf("TEST #%d\n", i+1);
        printf("zaffiri = %d [%d], rubini = %d [%d], topazi = %d [%d], smeraldi = %d [%d], TOT=%d {max_rip=%d}\n", pietre[0], prezzi[0], pietre[1], prezzi[1], pietre[2], prezzi[2], pietre[3], prezzi[3], tot, maxrip);
        printf("La soluzione ottima di valore %d usando  %d gemma/e e' :\n", prezzo_max, len);
        printf("-");
        for(j=0; j<len; j++){
            switch (sol[j])
            {
            case 0:
                printf("z-");
                break;
            case 1:
                printf("r-");
                break;
            case 2:
                printf("t-");
                break;
            case 3:
                printf("s-");
                break;
            default:
                break;
            }
        }
        printf("\n");
        free(sol);
        free(scelte);
    }
    return 0;
}

int ricerca_max(int *pietre, int *scelte, int pietre_distinte, int tot, int *sol_max, int *prezzi, int maxrip, int *len){
    int k, prezzo_max=0, i, *pietre_copy, *sol_k, j;

    pietre_copy = (int *) malloc(pietre_distinte*sizeof(int));
    for(k=tot; k>0; k--){   //trovo la soluzione valida massima per ogni k
        n_smer=0;
        n_zaff=0;
        for(i=0; i<pietre_distinte; i++) //setto nuovamente le pietre
            pietre_copy[i]=pietre[i]; 
        sol_k = (int *) malloc(k*sizeof(int));   
        prezzo_k = disp_rip(0, pietre_copy, scelte, sol_max, pietre_distinte, k, prezzi, maxrip, sol_k); //controllo se è maggiore del max
            if(prezzo_k > prezzo_max){
            prezzo_max = prezzo_k;
            *len = k;
        }
        free(sol_k);
    }
    free(pietre_copy);
    return prezzo_max;
}


int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k, int *prezzi, int maxrip, int *sol_k){
    int i, prezzo, j;
    if(pos==k){
            prezzo = calcola_prezzo(sol_k, k, prezzi);
            if((prezzo > prezzo_k) && (n_smer >= n_zaff)){
                prezzo_k = prezzo;
                for(j=0; j<k; j++){
                    sol[j]=sol_k[j]; 
                }
        }
        return prezzo_k; //soluzione valida di lunghezza k
    }
    for(i=0; i<n; i++){
        sol_k[pos] = scelte[i];
        if(is_valid(pietre, sol_k, pos,k, maxrip, prezzi)){ //pruning (controllo la validità della soluzione parziale)
            prezzo_k = disp_rip(pos+1, pietre, scelte, sol, n, k, prezzi, maxrip, sol_k);
            pietre[sol_k[pos]]++; 
            if(sol_k[pos]==0)
                n_zaff--;
            if(sol_k[pos]==3)
                n_smer--;
        }
    }
    return prezzo_k;
}


int is_valid(int *pietre, int *sol, int pos, int k, int maxrip, int *prezzi){

    switch (sol[pos]) // controllo se ci sono ancora pietre di quel tipo e se possono stare li
        {
        case 0:
            if(((sol[pos-1]!=0  && sol[pos-1]!=2) && (pos!=0)) || (pietre[0]==0) || (check_rip(sol,pos,maxrip))) //zaffiro deve essere preceduto da uno zaffiro o topazio
                return 0;
            n_zaff++;
            pietre[0]--;
            break;
        case 1:
            if(((sol[pos-1]!=0  && sol[pos-1]!=2)  && (pos!=0)) || (pietre[1]==0) || (check_rip(sol,pos,maxrip))) //rubino deve essere preceduto da uno zaffiro o topazio
                return 0;
            pietre[1]--; 
            break;
        case 2:
            if(((sol[pos-1]!=1  && sol[pos-1]!=3) && (pos!=0)) || (pietre[2]==0) || (check_rip(sol,pos,maxrip))) //topazio deve essere preceduto da un rubino o uno smeraldo
                return 0;
            pietre[2]--;
            break;
        case 3:
            if(((sol[pos-1]!=1  && sol[pos-1]!=3) && (pos!=0)) || (pietre[3]==0) || (check_rip(sol,pos,maxrip))) //smeraldo deve essere preceduto da uno smeraldo o rubino
                return 0;
            n_smer++;
            pietre[3]--;
            break;
        default:
            return 0;
        }
    return 1;
}

int check_rip(int *sol, int pos, int maxrip){
    int i;

    if(pos<maxrip) //non possono esserci maxrip pietre uguali 
        return 0;

    for(i=pos-maxrip; i<pos; i++)
        if(sol[i]!=sol[pos])
            return 0;

    return 1; //sequenza non valida
}

int calcola_prezzo(int *collana, int len, int *prezzi){
    int i, prezzo=0;
    for(i=0; i<len; i++){
        switch (collana[i])
        {
        case 0:
            prezzo += prezzi[0];
            break;
        case 1:
            prezzo += prezzi[1];
            break;
        case 2:
            prezzo += prezzi[2];
            break;
        case 3:
            prezzo += prezzi[3];
            break;
        default:
            break;
        }
    }
    return prezzo;
}