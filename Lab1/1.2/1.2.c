#include <stdio.h>
#include <string.h>
#define MAXLEN 31
#define MAXROWS 1000
#define NOMEFILE "corse.txt"

//formattazione riga file
// <codice_tratta><partenza><destinazione><data><ora_partenza><ora_arrivo><ritardo(minuti in interi)>

enum comando{r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine};

typedef struct tratta{
    char codice[MAXLEN], partenza[MAXLEN], arrivo[MAXLEN];
    int anno, mese, giorno;
    int oraA, oraP, minA, minP, secA, secP, ritardo;
}tratta_t;

enum comando leggiComando();
int selezionaDati(tratta_t *tabella, int N, enum comando scelta);
void riempiTabella(tratta_t *tabella, int N, FILE *filepointer);
void stampaTratta(tratta_t *tabella, int i);
void option1(tratta_t *tabella, int  N);
void option2(tratta_t *tabella, int  N);
void option3(tratta_t *tabella, int  N);
void option4(tratta_t *tabella, int  N);
void option5(tratta_t *tabella, int  N);

 int main() {
    enum comando scelta;
    tratta_t table[MAXROWS];
    int righe, end=0;
    FILE *fp;

    fp = fopen(NOMEFILE,"r");
    if(fp==NULL){
        printf("FILE NON TROVATO");
        return 0;
    }

    fscanf(fp, "%d", &righe);
    riempiTabella(table, righe, fp);

    while (end==0) {
        scelta = leggiComando();
        end = selezionaDati(table, righe, scelta);
    }

    return 0;
}

enum comando leggiComando(){
    enum comando scelta;
    int n;

    printf("MENU':\n"
            "1. elencare tutte le corse partite in un certo intervallo di date\n"
           "2. elencare tutti le corse partite da una certa fermata\n"
           "3. elencare tutti le corse che fanno capolinea in una certa fermata\n"
           "4. elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date\n"
           "5. elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\n"
           "6. fine\n"
           "Si inserisca l'intero relativo all'opzione che si vuole eseguire: ");
    scanf("%d", &n);
    switch (n){
        case 1:
            return r_date;
        case 2:
            return r_partenza;
        case 3:
            return r_capolinea;
        case 4:
            return r_ritardo;
        case 5:
            return r_ritardo_tot;
        default:
            return r_fine;
    }
}

int selezionaDati(tratta_t *tabella, int N, enum comando scelta){
     int end=0;
     switch (scelta) {
            case r_date:
                option1(tabella, N);
                break;
            case r_partenza:
                option2(tabella, N);
                break;
            case r_capolinea:
                option3(tabella, N);
                break;
            case r_ritardo:
                option4(tabella, N);
                break;
            case r_ritardo_tot:
                option5(tabella, N);
                break;
            case r_fine:
                end=1;
                break;
            default:
                break;
        }
    return end;
}

void riempiTabella(tratta_t *tabella, int N, FILE *filepointer){
    int i;
    for(i=0; i<N; i++){
        fscanf(filepointer, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", tabella[i].codice, tabella[i].partenza, tabella[i].arrivo, &tabella[i].anno, &tabella[i].mese, &tabella[i].giorno, &tabella[i].oraP, &tabella[i].minP, &tabella[i].secP, &tabella[i].oraA, &tabella[i].minA, &tabella[i].secA, &tabella[i].ritardo);
    }
 }

 void option1(tratta_t *tabella, int  N){
    int annoIn, meseIn, giornoIn, annoFi, meseFi, giornoFi, i;
     printf("Inserisci le di date inizio e fin su cui filtrare (AAAA/MM/GG): ");
     scanf("%d/%d/%d %d/%d/%d", &annoIn, &meseIn, &giornoIn,&annoFi, &meseFi, &giornoFi);
     for(i=0; i<N; i++){
         if(annoIn<tabella[i].anno || (annoIn==tabella[i].anno && meseIn<tabella[i].mese) || (annoIn==tabella[i].anno && meseIn==tabella[i].mese && giornoIn<tabella[i].giorno)){
             if(annoFi>tabella[i].anno || (annoFi==tabella[i].anno && meseFi>tabella[i].mese) || (annoFi==tabella[i].anno && meseFi==tabella[i].mese && giornoFi>tabella[i].giorno)){
                 stampaTratta(tabella, i);
             }
         }
     }
}

void option2(tratta_t *tabella, int  N){
    char fermata[MAXLEN];
    int i;
    printf("Inserisci la fermata di partenza: ");
    scanf("%s", fermata);
    for(i=0; i<N; i++){
        if(strcmp(fermata,tabella[i].partenza)==0){
            stampaTratta(tabella, i);
        }
    }
}

void option3(tratta_t *tabella, int  N){
    char fermata[MAXLEN];
    int i;
    printf("Inserisci la fermata di capolinea: ");
    scanf("%s", fermata);
    for(i=0; i<N; i++){
        if(strcmp(fermata,tabella[i].arrivo)==0){
            stampaTratta(tabella, i);
        }
    }
}

void option4(tratta_t *tabella, int  N){
    int annoIn, meseIn, giornoIn, annoFi, meseFi, giornoFi, i;
    printf("Inserisci le di date inizio e fin su cui filtrare (AAAA/MM/GG): ");
    scanf("%d/%d/%d %d/%d/%d", &annoIn, &meseIn, &giornoIn,&annoFi, &meseFi, &giornoFi);
    for(i=0; i<N; i++){
        if(annoIn<tabella[i].anno || (annoIn==tabella[i].anno && meseIn<tabella[i].mese) || (annoIn==tabella[i].anno && meseIn==tabella[i].mese && giornoIn<tabella[i].giorno)){
            if(annoFi>tabella[i].anno || (annoFi==tabella[i].anno && meseFi>tabella[i].mese) || (annoFi==tabella[i].anno && meseFi==tabella[i].mese && giornoFi>tabella[i].giorno)){
                if(tabella[i].ritardo!=0){
                    stampaTratta(tabella, i);
                }
            }
        }
    }

}

void option5(tratta_t *tabella, int  N){
     int i, ritardo=0;
     char codice[MAXLEN];
    printf("Inserisci il codice della tratta: ");
    scanf("%s", codice);
    for(i=0; i<N; i++){
        if(strcmp(codice, tabella[i].codice)==0){
            ritardo= ritardo + tabella[i].ritardo;
        }
    }
    printf("Il ritardo della tratta %s e' %d\n", codice, ritardo);
}

void stampaTratta(tratta_t *tabella, int i){
    printf("%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", tabella[i].codice, tabella[i].partenza, tabella[i].arrivo, tabella[i].anno, tabella[i].mese, tabella[i].giorno, tabella[i].oraP, tabella[i].minP, tabella[i].secP, tabella[i].oraA, tabella[i].minA, tabella[i].secA, tabella[i].ritardo);
 }
