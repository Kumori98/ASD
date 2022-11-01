#include <stdio.h>
int gcd(int a, int b);

int main() {
    int x, y, mcd;
    printf("Inserisci due interi di cui sai vuole sapere il MCD: ");
    scanf("%d %d", &x, &y);
    mcd = gcd(x,y);

    printf("Il massimo comune divisore di %d e %d e': %d", x, y, mcd);


    return 0;
}

int gcd(int a, int b){
    if(a==b){
        return a;
    }
    if(a>b){
        if(a%2==0 && b%2==0){ //entrambi pari
            return 2* gcd(a/2,b/2);
        }
        if(a%2!=0 && b%2!=0){ //entrambi dispari
            return gcd((a-b)/2, b);
        }
        if(a%2!=0 && b%2==0){
            return gcd(a, b/2);
        }
        if(a%2==0 && b%2!=0){
            return gcd(a/2, b);
        }
    }
    else {

        if(a%2==0 && b%2==0){ //entrambi pari
            return 2* gcd(b/2,a/2);
        }
        if(a%2!=0 && b%2!=0){ //entrambi dispari
            return gcd((b-a)/2, a);
        }
        if(a%2!=0 && b%2==0){
            return gcd(a, b/2);
        }
        if(a%2==0 && b%2!=0){
            return gcd(a/2, b);
        }
    }
}