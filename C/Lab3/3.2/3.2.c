#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOMEFILE "brani.txt"


typedef struct Livello{
    char **scelte; //titoli delle canzoni[num_scelte][255]
    int num_scelte;} Livello;
    
int princ_molt(int pos, Livello *canzoni, char **sol, int k, int count);
void carica_canzoni(int num_amici, FILE *fp, Livello *canzoni);


int main(){
    FILE *fp;
    int num_amici, count, i, j;
    Livello *canzoni;
    char **sol;

    fp=fopen(NOMEFILE,"r");
    if(fp==NULL){
        printf("File non trovato;");
        return -1;
    }

    fscanf(fp,"%d", &num_amici);
    canzoni = (Livello *) malloc(num_amici*sizeof(Livello));
    
    sol = (char **) malloc(num_amici*sizeof(char*));
    for(i=0; i<num_amici; i++)
        sol[i] = (char *) malloc(255*sizeof(char));
    

    carica_canzoni(num_amici,fp,canzoni);
    count = princ_molt(0, canzoni, sol, num_amici, 0);
    printf("Le possibili combinazioni sono %d \n", count);

    for(i=0; i<num_amici; i++){
        for(j=0; j<canzoni[i].num_scelte; j++)
            free(canzoni[i].scelte[j]);
        free(canzoni[i].scelte);
    }

    fclose(fp);

    for(i=0; i<num_amici; i++)
        free(sol[i]);
    free(sol);
    
    free(canzoni);
    
    return 0;
}

void carica_canzoni(int num_amici, FILE *fp, Livello *canzoni){
    int i, j;

    for(i=0; i<num_amici; i++){
        fscanf(fp, "%d", &canzoni[i].num_scelte);
        canzoni[i].scelte = (char **) malloc(canzoni[i].num_scelte*sizeof(char*));
        for(j=0; j<canzoni[i].num_scelte; j++){
            canzoni[i].scelte[j] = (char *) malloc(255*sizeof(char));
            fscanf(fp, "%s", canzoni[i].scelte[j]);
        }
    }
}

int princ_molt(int pos, Livello *canzoni, char **sol, int k, int count){
  int i;
  if (pos >= k) {
    for (i = 0; i < k; i++)
      printf("%s ", sol[i]);
    printf("\n");
    return count+1;
  }
  for (i = 0; i < canzoni[pos].num_scelte; i++) {
    strcpy(sol[pos],canzoni[pos].scelte[i]);
    count = princ_molt(pos+1, canzoni, sol, k, count);
  }
  return count;
}