#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "hard_test_set.txt"

int ricerca_max(int *pietre, int *scelte, int pietre_distinte, int tot, int *sol);
int is_valid(int *pietre, int *sol, int pos, int k);
int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k);

int main(){
    FILE *fp;
    int n_test, i, pietre[4], tot, max, *sol, j, pietre_dist=0, *scelte;



    fp = fopen(NOMEFILE, "r");
    if(fp==NULL){
        printf("File non trovato");
        return -1;
    }

    fscanf(fp, "%d", &n_test);

    for(i=0; i<n_test; i++){
        pietre_dist=0;
        tot=0;
        for(j=0; j<4; j++){
            fscanf(fp, "%d", &pietre[j]);
            if(pietre[j]!=0)
                pietre_dist++; 
            tot += pietre[j];
        }
        scelte = (int *) malloc(pietre_dist*sizeof(int));
        for(j=0; j<pietre_dist;j++)
            scelte[j]=j;
        sol = (int *) malloc(tot*sizeof(int)); //soluzione massima di dimensione tot
        max = ricerca_max(pietre, scelte, pietre_dist, tot, sol);
        printf("TEST #%d\n", i+1);
        printf("zaffiri = %d, rubini = %d, topazi = %d, smeraldi = %d, TOT=%d \n", pietre[0], pietre[1], pietre[2], pietre[3], tot);
        printf("La collana massima,di lunghezza %d, e' :\n", max);
        printf("-");
        for(j=0; j<max; j++){
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

int ricerca_max(int *pietre, int *scelte, int pietre_distinte, int tot, int *sol){
    int k, max, i, *pietre_copy;
    
    pietre_copy = (int *) malloc(pietre_distinte*sizeof(int));
    for(k=tot; k>0; k--){   //trovo la soluzione valida massima per ogni k
        for(i=0; i<pietre_distinte; i++) //setto nuovamente le pietre
            pietre_copy[i]=pietre[i];
        max = disp_rip(0, pietre_copy, scelte, sol, pietre_distinte, k);
        if(max != -1)
            break;
    }
    free(pietre_copy);
    return max;
}

int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k){
    int i, max=-1;
    if(pos==k)
            return k; //soluzione valida di lunghezza k
    for(i=0; i<n && max==-1; i++){
        sol[pos] = scelte[i];
        if(is_valid(pietre,sol,pos,k)){ //pruning (controllo la validità della soluzione parziale)
            max = disp_rip(pos+1, pietre, scelte, sol, n, k);
            if(max==-1)
                pietre[sol[pos]]++; 
        }
    }
    return max;
}

int is_valid(int *pietre, int *sol, int pos, int k){

        switch (sol[pos]) // controllo se ci sono ancora pietre di quel tipo e se possono stare li
        {
        case 0:
            if(((sol[pos-1]!=0  && sol[pos-1]!=2) && (pos!=0)) || pietre[0]==0) //zaffiro deve essere preceduto da uno zaffiro o topazio
                return 0;
            pietre[0]--;
            break;
        case 1:
            if(((sol[pos-1]!=0  && sol[pos-1]!=2)  && (pos!=0)) || pietre[1]==0) //rubino deve essere preceduto da uno zaffiro o topazio
                return 0;    
            pietre[1]--; 
            break;
        case 2:
            if(((sol[pos-1]!=1  && sol[pos-1]!=3) && (pos!=0)) || pietre[2]==0) //topazio deve essere preceduto da un rubino o uno smeraldo
                return 0;
            pietre[2]--;
            break;
        case 3:
            if(((sol[pos-1]!=1  && sol[pos-1]!=3) && (pos!=0)) || pietre[3]==0) //smeraldo deve essere preceduto da uno smeraldo o rubino
                return 0;
            pietre[3]--;
            break;
        default:
            return 0;
        }
    return 1;
}