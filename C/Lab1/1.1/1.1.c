#include <stdio.h>
#include <ctype.h>

#define N 100
#define M 30

char *cercaRegexp(char *src, char *regexp);
int reglen(char *regexp);
int checkReg(char *src, char *regexp, int srcIndex, int regIndex);

int main() {
    char stringa[N], regexp[M], *risultato = NULL;

    printf("Inserisci stringa e regexp:  ");
    scanf("%s %s", stringa, regexp);

    risultato = cercaRegexp(stringa, regexp);
    if(risultato == NULL){
        printf("Occorrenza non trovata");
    }
    return 0;
}

char *cercaRegexp(char *src, char *regexp){
    char *occ;
    int regLen, i=0, j=0, p, incremento, found=1, t;

    regLen= reglen(regexp);

    while(src[i]!='\0'){
        p = i;
        while(regexp[j]!='\0' && found==1){
            if(isalnum(regexp[j])==0) {
                incremento = checkReg(src, regexp, p, j);
                if (incremento == 0) {
                    j = 0;
                    found=0;
                }
                else {
                    j = j + incremento;
                    p++;
                }
            }
            else if(regexp[j]==src[p]){
                p++;
                j++;
            }
            else{
                j = 0;
                found=0;
            }
        }
        if(found==1){
            occ= &src[i];
            printf("Occorrenza trovata: ");
            for(t=i; t<regLen+i; t++){
                printf("%c",src[t]);
            }
            return occ;
        }
        else {
            i++;
            found=1;
        }
    }
    return NULL;
}


int reglen(char *regexp){
    int n=0, i=0;

    while(regexp[i]!='\0'){

        if(regexp[i]=='.'){
            n++;
            i++;
        }
        else if(regexp[i]=='\\'){
            i=i+2;
            n++;
            }
        else if(regexp[i]=='['){
            i++;
            while(regexp[i]!=']'){
                i++;
            }
            i++;
            n++;
        }
        else {
            n++;
            i++;
        }
    }
    return n;
}

int checkReg(char *src, char *regexp, int srcIndex, int regIndex){
    int flag=0, incremento=0;

    if(regexp[regIndex]=='.'){
        return 1;
    }
    else if(regexp[regIndex]=='\\') {
        regIndex++;
        if (regexp[regIndex]=='a' && islower(src[srcIndex])!=0){
            return 2;
        } else if (regexp[regIndex]=='A' && isupper(src[srcIndex])!=0) {
            return 2;
        } else {
            return 0;
        }
    }
    else if(regexp[regIndex]=='['){
        regIndex++;
        incremento++;
        if(regexp[regIndex]=='^'){
            regIndex++;
            incremento=incremento+2;
            while(regexp[regIndex]!=']'){
                if(src[srcIndex]!=regexp[regIndex]){
                    incremento++;
                    regIndex++;
                    flag=1;
                }
                else {
                    regIndex++;
                    incremento++;
                }
            }
            if(flag==0) return 0;
        }
        else {
            incremento++;
            while(regexp[regIndex]!=']'){
                if(src[srcIndex]==regexp[regIndex]){
                    regIndex++;
                    incremento++;
                    flag=1;
                }
                else{
                    regIndex++;
                    incremento++;
                }
            }
            if(flag==0) return 0;
        }
    }
    return incremento;
}
