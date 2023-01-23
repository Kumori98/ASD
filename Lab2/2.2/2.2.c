#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "mat.txt"

int **malloc2dR(int nr, int nc);
void riempiMat(int **mat, int nr, int nc, FILE *fp);
void separa(int **mat, int nr, int nc, int *white, int *black);

int main() {
    int nr, nc, **mat, *black, *white, ncaselle, i;
    FILE *fp;

    fp= fopen(NOMEFILE,"r");
    if(fp==NULL){
        printf("File non trovato!");
        return -1;
    }

    fscanf(fp,"%d %d", &nr, &nc);

    mat = malloc2dR(nr,nc);
    riempiMat(mat,nr,nc,fp);

    fclose(fp);

    ncaselle = (nr*nc)/2;

    white = (int *) malloc(ncaselle*(sizeof (int)));
    black = (int *) malloc(ncaselle*(sizeof (int)));

    separa(mat,nr,nc,white,black);

    printf("Il vettore con le caselle bianche:\n");
    for(i=0; i<ncaselle; i++)
        printf("%d ", white[i]);
    printf("\nIl vettore con le caselle nere:\n");
    for(i=0; i<ncaselle; i++)
        printf( "%d ", black[i]);

    for(i=0; i<nr; i++)
        free(mat[i]);
    free(mat);    


    free(white);
    free(black);

    return 0;
}

int **malloc2dR(int nr, int nc){
    int **mat, i;

    mat = (int **) malloc(nr*(sizeof (int *)));
    for(i=0; i<nc; i++){
        mat[i] = (int *) malloc(nc*(sizeof(int)));
    }
    if (mat==NULL)
        exit(1);
    return mat;
}

void riempiMat(int **mat,int nr, int nc, FILE *fp){
    int i, j;
    for(i=0;i<nr;i++){
        for(j=0;j<nc;j++){
            fscanf(fp, "%d", &mat[i][j]);
        }
    }
}

void separa(int **mat, int nr, int nc, int *white, int *black){
    int i, j, indexb=0, indexw=0;
    for(i=0;i<nr;i++){
        for(j=0;j<nc;j++){
            if((i+j)%2==0){
                black[indexb] = mat[i][j];
                indexb++;
            }
            else {
                white[indexw] = mat[i][j];
                indexw++;
            }
        }
    }
}