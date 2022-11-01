#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "vector.txt"

int majority( int *a, int N);
int majorityR(int *a, int l, int r);
int occorrenze(int *a, int l, int r, int val);


int main(){
    int *vector, N, i, maggioritario;
    FILE *fp;

    fp = fopen(NOMEFILE,"r");
    fscanf(fp,"%d", &N);
    if(fp==NULL){
        printf("File non trovato");
        return -1;
    }
    vector = (int *) malloc(N*sizeof(int));
    for (i=0; i<N; i++)
        fscanf(fp,"%d", &vector[i]);

    fclose(fp);

    maggioritario = majority(vector, N);

    printf("Nel vettore: ");
    for(i=0; i<N; i++)
        printf("%d ", vector[i]);
    if(maggioritario!=-1)
        printf("il maggioritario e': %d", maggioritario);
    else
        printf("non e' presente un maggioritario");

    free(vector);
    return 0;
}

int majority( int *a, int N){  //wrapper
    int l=0, r=N-1;
    return majorityR(a,l,r);
}

int majorityR(int *a, int l, int r){
    int m=(l+r)/2, i, mL, mR, occL, occR;
    if(l>=r)  {             //unico elemento allora è maggioritario
        return a[l];
    } 

    mL = majorityR(a,l,m);
    mR = majorityR(a,m+1,r); 

    if(mR==mL) //due maggioritati uguali
        return mR;

    else{
        if(mR==-1){ //a destra non ho maggioritario, a sinistra si
            occL = occorrenze(a,l,r,mL);
            if(occL>(r-l)/2)
                return mL;
            else
                return -1;
        }

        else if(mL==-1){ //a sinistra non ho maggioritario, a destra si
            occR = occorrenze(a,l,r,mR);
            if(occR>(r-l)/2)
                return mR;
            else
                return -1;
        }
        
        else if(r-l>=2){ //ho due maggioritari diversi a sinistra e destra (con vettore di dimensione maggiore di 2)
            occL = occorrenze(a,l,r,mL);
            if(occL>(r-l)/2)
                return mL;
            else{
                occR = occorrenze(a,l,r,mR);
                if(occR>(r-l)/2)
                    return mR;
            }
        }
        return -1;
    }   
}

int occorrenze(int *a, int l, int r, int val){
   int i,res=0;
    for(i=l; i<=r; i++){
        if(a[i]==val)
            res++;
    }
    return res;
}