#include <stdio.h>
#include <string.h>
#define MAXLEN 31
#define MAXROWS 1000
#define NOMEFILEIN "corse.txt"
#define NOMEFILEOUT "log.txt"



enum comando{r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_ordina_data, r_ordina_codice, r_ordina_partenza, r_ordina_arrivo, r_fine};

typedef struct tratta{
    char codice[MAXLEN], partenza[MAXLEN], arrivo[MAXLEN];
    int anno, mese, giorno;
    int oraA, oraP, minA, minP, secA, secP, ritardo;
}tratta_t;

enum comando leggiComando();
int selezionaDati(tratta_t *tabella, int N, enum comando scelta, FILE *log, tratta_t *byDate[MAXROWS],tratta_t *byCode[MAXROWS], tratta_t *byStart[MAXROWS], tratta_t *byEnd[MAXROWS]);
void riempiTabella(tratta_t *tabella, int N, FILE *filepointer);
void stampaTratta(tratta_t *tabella, int i, FILE *log, int destinazione);
void option1(tratta_t *tabella, int N, FILE *log);
void option2(tratta_t *tabella, int N, FILE *log);
void option3(int N, FILE *log, tratta_t *order[MAXROWS]);
int binarySearch(tratta_t *order[MAXROWS], char destinazione[MAXLEN], int low, int high);
void option4(tratta_t *tabella, int N, FILE *log);
void option5(tratta_t *tabella, int N, FILE *log);
void option6(int N, FILE *log, tratta_t *order[MAXROWS]);
void option7_8_9(int N, FILE *log, tratta_t *order[MAXROWS], int criterio);
void sortByString(tratta_t *order[],int N, int criterio);
void sortByDate(tratta_t *order[],int N);
int bisestile(int anno);
int calcoloGiorni(int anno, int mese, int giorno);
int calcoloSecondi(int ora, int minuti, int secondi);
void stampaTrattaReference(tratta_t *order[], int i, FILE *log, int destinazione);

int main() {
    enum comando scelta;
    tratta_t table[MAXROWS], *byDate[MAXROWS], *byStart[MAXROWS], *byEnd[MAXROWS], *byCode[MAXROWS];
    int righe, end=0, i;
    FILE *fp;
    FILE *fpOut;

    fp = fopen(NOMEFILEIN,"r");
    if(fp==NULL){
        printf("FILE INPUT NON TROVATO");
        return 0;
    }

    fpOut = fopen(NOMEFILEOUT,"w");
    if(fp==NULL){
        printf("FILE OUTPUT NON TROVATO");
        return 0;
    }

    fscanf(fp, "%d", &righe);
    riempiTabella(table, righe, fp);

    for(i=0; i<righe; i++){
        byDate[i] = &table[i];
    }
    for(i=0; i<righe; i++){
        byCode[i] = &table[i];
    }
    for(i=0; i<righe; i++){
        byStart[i] = &table[i];
    }
    for(i=0; i<righe; i++){
        byEnd[i] = &table[i];
    }

    while (end==0) {
        scelta = leggiComando();
        end = selezionaDati(table, righe, scelta, fpOut, byDate, byCode, byStart, byEnd);
    }

    fclose(fp);
    fclose(fpOut);

    return 0;
}

enum comando leggiComando(){
    int n;

    printf("MENU':\n"
           "1. elencare tutte le corse partite in un certo intervallo di date\n"
           "2. elencare tutti le corse partite da una certa fermata\n"
           "3. elencare tutti le corse che fanno capolinea in una certa fermata\n"
           "4. elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date\n"
           "5. elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\n"
           "6. ordinare le corse per data, e a parita' di data per ora\n"
           "7. ordinare le corse per codice di tratta\n"
           "8. ordinare le corse per stazione di partenza\n"
           "9. ordinare le corse per stazione di arrivo\n"
           "10. fine\n"
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
        case 6:
            return r_ordina_data;
        case 7:
            return r_ordina_codice;
        case 8:
            return r_ordina_partenza;
        case 9:
            return r_ordina_arrivo;
        default:
            return r_fine;
    }
}

int selezionaDati(tratta_t *tabella, int N, enum comando scelta, FILE *log, tratta_t *byDate[MAXROWS],tratta_t *byCode[MAXROWS], tratta_t *byStart[MAXROWS], tratta_t *byEnd[MAXROWS] ){
    int end=0;
    switch (scelta) {
        case r_date:
            option1(tabella, N, log);
            break;
        case r_partenza:
            option2(tabella, N, log);
            break;
        case r_capolinea:
            option3(N, log, byEnd);
            break;
        case r_ritardo:
            option4(tabella, N, log);
            break;
        case r_ritardo_tot:
            option5(tabella, N, log);
            break;
        case r_ordina_data:
            option6(N, log, byDate);
            break;
        case r_ordina_codice:
            option7_8_9(N, log, byCode,0);
            break;
        case r_ordina_partenza:
            option7_8_9(N, log, byStart,1);
            break;
        case r_ordina_arrivo:
            option7_8_9(N, log, byEnd,2);
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

void option1(tratta_t *tabella, int  N, FILE *log){
    int annoIn, meseIn, giornoIn, annoFi, meseFi, giornoFi, i, destinazione;
    printf("Inserisci le di date inizio e fin su cui filtrare (AAAA/MM/GG): ");
    scanf("%d/%d/%d %d/%d/%d", &annoIn, &meseIn, &giornoIn,&annoFi, &meseFi, &giornoFi);
    printf("1. Stampi il risultato a video;\n"
           "2. Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    for(i=0; i<N; i++){
        if(annoIn<tabella[i].anno || (annoIn==tabella[i].anno && meseIn<tabella[i].mese) || (annoIn==tabella[i].anno && meseIn==tabella[i].mese && giornoIn<tabella[i].giorno)){
            if(annoFi>tabella[i].anno || (annoFi==tabella[i].anno && meseFi>tabella[i].mese) || (annoFi==tabella[i].anno && meseFi==tabella[i].mese && giornoFi>tabella[i].giorno)){
                stampaTratta(tabella, i, log, destinazione);
            }
        }
    }
}

void option2(tratta_t *tabella, int  N, FILE *log){ //ricerca lineare
    char fermata[MAXLEN];
    int i, destinazione, len;
    printf("Inserisci la fermata di partenza: ");
    scanf("%s", fermata);
    len = strlen(fermata);
    printf("1. Stampi il risultato a video;\n"
           "2. Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    for(i=0; i<N; i++){
        if(strncmp(fermata,tabella[i].partenza,len)==0){
            stampaTratta(tabella, i, log, destinazione);
        }
    }
}

void option3( int  N, FILE *log, tratta_t  *order[MAXROWS]){ //con ricerca dicotomica
    char fermata[MAXLEN];
    int destinazione, result=1,j;
    tratta_t *ordercopy[MAXROWS];

    option7_8_9(N, log, order,3);
    for(j=0;j<N;j++)
        ordercopy[j]=order[j];
    printf("Inserisci la fermata di capolinea: ");
    scanf("%s", fermata);
    printf("1. Stampi il risultato a video;\n"
           "2. Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    while(result != -1) {
        result = binarySearch(ordercopy, fermata, 0, N);
        if (result != -1) {
            stampaTrattaReference(ordercopy, result, log, destinazione);
            strcpy(ordercopy[result]->arrivo,"USED");
            option7_8_9(N, log, ordercopy,3);
        }
    }
}

int binarySearch(tratta_t *order[MAXROWS], char destinazione[MAXLEN], int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (strcmp(destinazione,order[mid]->arrivo)==0)
            return mid;

        if (strcmp(destinazione,order[mid]->arrivo)>0)
            low = mid + 1;

        else
            high = mid - 1;
    }
    return -1;
}


void option4(tratta_t *tabella, int  N, FILE *log){
    int annoIn, meseIn, giornoIn, annoFi, meseFi, giornoFi, i, destinazione;
    printf("Inserisci le di date inizio e fin su cui filtrare (AAAA/MM/GG): ");
    scanf("%d/%d/%d %d/%d/%d", &annoIn, &meseIn, &giornoIn,&annoFi, &meseFi, &giornoFi);
    printf("1. Stampi il risultato a video;\n"
           "2. Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    for(i=0; i<N; i++){
        if(annoIn<tabella[i].anno || (annoIn==tabella[i].anno && meseIn<tabella[i].mese) || (annoIn==tabella[i].anno && meseIn==tabella[i].mese && giornoIn<tabella[i].giorno)){
            if(annoFi>tabella[i].anno || (annoFi==tabella[i].anno && meseFi>tabella[i].mese) || (annoFi==tabella[i].anno && meseFi==tabella[i].mese && giornoFi>tabella[i].giorno)){
                if(tabella[i].ritardo!=0){
                    stampaTratta(tabella, i, log, destinazione);
                }
            }
        }
    }
}

void option5(tratta_t *tabella, int  N, FILE *log){
    int i, ritardo=0, destinazione;
    char codice[MAXLEN];
    printf("Inserisci il codice della tratta: ");
    scanf("%s", codice);
    for(i=0; i<N; i++){
        if(strcmp(codice, tabella[i].codice)==0){
            ritardo= ritardo + tabella[i].ritardo;
        }
    }
    printf("1. Stampi il risultato a video;\n"
           "2.Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    if(destinazione==1){
        printf("Il ritardo della tratta %s e' %d\n", codice, ritardo);
    }
    else{
        fprintf(log, "Il ritardo della tratta %s e' %d\n", codice, ritardo);
    }
}

void option6(int N, FILE *log, tratta_t *order[MAXROWS]){
    int destinazione, i;
    sortByDate(order, N);
    printf("1. Stampi il risultato a video;\n"
           "2. Salvi il risultato su file;\n"
           "Si inserisca l'intero relativo all'opzione scelta:");
    scanf("%d",&destinazione);
    for(i=0; i<N; i++){
        stampaTrattaReference(order,i,log,destinazione);
    }
}

void option7_8_9(int N, FILE *log, tratta_t *order[MAXROWS], int criterio){
    int i, destinazione;
    sortByString(order,N,criterio);
    if(criterio!=3) {
        printf("1. Stampi il risultato a video;\n"
               "2. Salvi il risultato su file;\n"
               "Si inserisca l'intero relativo all'opzione scelta:");
        scanf("%d", &destinazione);
        for (i = 0; i < N; i++) {
            stampaTrattaReference(order, i, log, destinazione);
        }
    }
}


void stampaTratta(tratta_t *tabella, int i, FILE *log, int destinazione){
    if(destinazione==1){
        printf("%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", tabella[i].codice, tabella[i].partenza, tabella[i].arrivo, tabella[i].anno, tabella[i].mese, tabella[i].giorno, tabella[i].oraP, tabella[i].minP, tabella[i].secP, tabella[i].oraA, tabella[i].minA, tabella[i].secA, tabella[i].ritardo);
    }
    else{
        fprintf(log,"%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", tabella[i].codice, tabella[i].partenza, tabella[i].arrivo, tabella[i].anno, tabella[i].mese, tabella[i].giorno, tabella[i].oraP, tabella[i].minP, tabella[i].secP, tabella[i].oraA, tabella[i].minA, tabella[i].secA, tabella[i].ritardo);
    }
}

void stampaTrattaReference(tratta_t *order[], int i, FILE *log, int destinazione){
    if(destinazione==1){
        printf("%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", order[i]->codice, order[i]->partenza, order[i]->arrivo, order[i]->anno, order[i]->mese, order[i]->giorno, order[i]->oraP, order[i]->minP, order[i]->secP, order[i]->oraA, order[i]->minA, order[i]->secA, order[i]->ritardo);
    }
    else{
        fprintf(log,"%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", order[i]->codice, order[i]->partenza, order[i]->arrivo, order[i]->anno, order[i]->mese, order[i]->giorno, order[i]->oraP, order[i]->minP, order[i]->secP, order[i]->oraA, order[i]->minA, order[i]->secA, order[i]->ritardo);
    }
}

void sortByString(tratta_t *order[],int N, int criterio){ //insertion sort (Stabile)
    int i, j, l=0, r=N-1;
    tratta_t *x;
    for (i = l+1; i <= r; i++) {
        switch (criterio) {
            case 0:
                x = order[i];
                j = i - 1;
                while (j >= l && strcmp(x->codice,order[j]->codice)<0){
                    order[j+1] = order[j];
                    j--;
                }
                order[j+1] = x;
                break;
            case 1:
                x = order[i];
                j = i - 1;
                while (j >= l && strcmp(x->partenza,order[j]->partenza)<0){
                    order[j+1] = order[j];
                    j--;
                }
                order[j+1] = x;
                break;
            case 2:
                x = order[i];
                j = i - 1;
                while (j >= l && strcmp(x->arrivo,order[j]->arrivo)<0){
                    order[j+1] = order[j];
                    j--;
                }
                order[j+1] = x;
                break;
            case 3:
                x = order[i];
                j = i - 1;
                while (j >= l && strcmp(x->arrivo,order[j]->arrivo)<0){
                    order[j+1] = order[j];
                    j--;
                }
                order[j+1] = x;
                break;
            default:
                break;
        }
    }
}


void sortByDate(tratta_t *order[],int N){
    int i, j, l=0, r=N-1, min;
    tratta_t *temp;
    for (i = l; i < r; i++) {
        min = i;
        for (j = i+1; j <= r; j++){
            if ( calcoloGiorni(order[j]->anno,order[j]->mese,order[j]->giorno) < calcoloGiorni(order[min]->anno,order[min]->mese,order[min]->giorno))
                min = j;
            else if(calcoloGiorni(order[j]->anno,order[j]->mese,order[j]->giorno) == calcoloGiorni(order[min]->anno,order[min]->mese,order[min]->giorno)){
                if(calcoloSecondi(order[j]->oraP,order[j]->minP,order[j]->secP) < calcoloSecondi(order[min]->oraP,order[min]->minP,order[min]->secP)){
                    min = j;
                }
            }
        }
        if (min != i) {
            temp = order[i];
            order[i] = order[min];
            order[min] = temp;
        }
    }
}


int bisestile(int anno)
    {
        return (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);     //gli anni non secolari il cui numero è divisibile per 4; gli anni secolari il cui numero è divisibile per 400.
    }

int calcoloGiorni(int anno, int mese, int giorno){
        static const int days[2][13] = {
                {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
                {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
        };
        int isBisestile = bisestile(anno);

        return days[isBisestile][mese] + giorno;
}
 int calcoloSecondi(int ora, int minuti, int secondi){
    return (ora*60*60) + (minuti*60) + secondi;
}


