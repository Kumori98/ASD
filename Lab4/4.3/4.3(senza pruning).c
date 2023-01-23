#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "very_easy_test_set.txt"

int ricerca_max(int *pietre, int *scelte, int pietre_distinte, int tot, int *sol);
int is_valid(int *pietre, int *sol, int len, int n);
int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k, int max);

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
        printf("zaffiri = %d, rubini = %d, smeraldi = %d, topazi = %d \n", pietre[0], pietre[1], pietre[2], pietre[3]);
        printf("La collana massima,di lunghezza %d, e' :\n", max);
        for(j=0; j<max; j++){
            switch (sol[j])
            {
            case 0:
                printf("-z-");
                break;
            case 1:
                printf("-r-");
                break;
            case 2:
                printf("-s-");
                break;
            case 3:
                printf("-t-");
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
    int k, max=-1, i;
    for(k=tot; k>0; k--){   //trovo la soluzione valida massima per ogni k
        max = disp_rip(0, pietre, scelte, sol, pietre_distinte, k, max);
        if(max != -1)
            break;
    }
    return max;
}

int disp_rip(int pos, int *pietre, int *scelte, int *sol, int n, int k, int max){
    int i,j;
    if(pos>=k){
        max = is_valid(pietre, sol, k, n); //controllo se valida
        return max;
    }
    for(i=0; i<n && max==-1; i++){
        sol[pos] = scelte[i];
        max = disp_rip(pos+1, pietre, scelte, sol, n, k, max);
    }
    return max;
}

int is_valid(int *pietre, int *sol, int len, int n){
    int i, *pietre_locale;

    pietre_locale = (int *) malloc(n*sizeof(int));
    for (i=0; i<n; i++) //copio il vettore cosi da non perdere i valori originali
        pietre_locale[i]=pietre[i];
    for(i=0; i<len; i++){
        switch (sol[i]) // controllo se ci sono ancora pietre di quel tipo e se possono stare li
        {
        case 0:
            pietre_locale[0]--;
            if(((sol[i+1]!=0  && sol[i+1]!=1) && (i!=len-1)) || pietre_locale[0]<0)
                return -1;
            break;
        case 1:
            pietre_locale[1]--;
            if(((sol[i+1]!=2  && sol[i+1]!=3) && (i!=len-1)) || pietre_locale[1]<0)
                return -1;            
            break;
        case 2:
            pietre_locale[2]--;
            if(((sol[i+1]!=2  && sol[i+1]!=3) && (i!=len-1)) || pietre_locale[2]<0)
                return -1;
            break;
        case 3:
            pietre_locale[3]--;
            if(((sol[i+1]!=0  && sol[i+1]!=1) && (i!=len-1)) || pietre_locale[3]<0)
                return -1;
            break;
        default:
            return -1;
        }
    }
    return len;
}



