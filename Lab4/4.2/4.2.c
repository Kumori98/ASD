#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 51
#define NOMEFILEOUT "out.txt"


typedef struct node *link;

typedef struct data{
    int anno, mese, giorno;
}data_t;

typedef struct Item{   
    int cap;
    char nome[MAXLEN], cognome[MAXLEN], via[MAXLEN], citta[MAXLEN], codice[MAXLEN];
    data_t data_di_nascita;
}item_t;

struct node{
    item_t val;
    link next;
};

link SortListIns(link h, item_t val);
link newNode(item_t val, link next);
int confronta_data(data_t data1, data_t data2);
link carica_da_file(link h);
link inserisci_da_tastiera(link h);
int stampa_menu();
item_t ricerca(link h);
item_t ITEMsetvoid();
void stampa_dato(item_t i, int mess_finale,FILE *fpout);
item_t cancella(link *hp);
void stampa_lista(link h);
void ciclo_cancella_date(link h);
item_t cancella_data(link *hp, data_t data1, data_t data2);
int is_void(item_t i);



int main(){
    link head = NULL;
    int scelta;

    do
    {
        scelta = stampa_menu();
    switch (scelta)
    {
    case 1:
        head = inserisci_da_tastiera(head);
        break;
    case 2:
        head = carica_da_file(head);
        break;
    case 3:
        stampa_dato(ricerca(head),0,NULL);
        break;
    case 4:
        stampa_dato(cancella(&head),0,NULL);
        printf("Cancellata!\n");
        break;
    case 5:
        ciclo_cancella_date(head);
        break;
    case 6:
        stampa_lista(head);
        break;


    default:
        scelta = 7;
    }
    } while (scelta!=7);

    return 0;
}

void ciclo_cancella_date(link h){
    data_t data1, data2;
    item_t i;

    printf("Inserisci la data d'inizio: ");
    scanf("%d/%d/%d", &data1.giorno, &data1.mese, &data1.anno);
    printf("Inserisci la data di fine: ");
    scanf("%d/%d/%d", &data2.giorno, &data2.mese, &data2.anno);

    do
    {
        i = cancella_data(&h, data1, data2);
        stampa_dato(i,1,NULL);
    } while (!is_void(i));
}

item_t cancella_data(link *hp, data_t data1, data_t data2){
    link *xp, t;
    item_t i = ITEMsetvoid();  

    for(xp = hp; (*xp)!=NULL; xp=&((*xp)->next)){
        if((confronta_data((*xp)->val.data_di_nascita,data1)) && (confronta_data(data2,(*xp)->val.data_di_nascita))){
            t = *xp;
            *xp = (*xp)->next;
            i = t->val;
            free(t);
            break;
        }
    }
    return i;
}

void stampa_lista(link h){
    link x;
    FILE *fpout;

    fpout = fopen(NOMEFILEOUT, "w");
    for(x=h; x!=NULL; x=x->next){
        stampa_dato(x->val,1,fpout);
    }
    fclose(fpout);
}

item_t cancella(link *hp){
    link *xp, t;
    char codice[MAXLEN];
    item_t i = ITEMsetvoid();

    printf("Inserisci il codice dell'anagrafica da cancellare: ");
    scanf("%s", codice);  

    for(xp = hp; (*xp)!=NULL; xp=&((*xp)->next)){
        if(strcmp((*xp)->val.codice, codice)==0){
            t = *xp;
            *xp = (*xp)->next;
            i = t->val;
            free(t);
            break;
        }
    }
    return i;
}

item_t ricerca(link h){
    link x;
    item_t i = ITEMsetvoid();
    char codice[MAXLEN];

    printf("Inserisci il codice della tratta da cercare: ");
    scanf("%s", codice);

    for(x=h; x!=NULL; x= x->next){
        if(strcmp(x->val.codice,codice)==0){
            i = x->val;
            break;
        }
    }
    return i;
}

int stampa_menu(){
    int scelta;
    printf("****************************************** MENU' ****************************************** \n");
    printf("1. Acquisizione ed inserimento ordinato di un nuovo elemento in lista da tastiera;\n");
    printf("2. Acquisizione ed inserimento ordinato di un nuovo elemento in lista da file;\n");
    printf("3. Ricerca, per codice, di un elemento;\n");
    printf("4. Cancellazione con estrazione, tramite codice, di un elemento dalla lista\n");
    printf("5. Cancellazione con estrazione di tutti gli elementi con date comprese tra 2 date\n");
    printf("6. Stampa della lista su file\n");
    printf("7. Terminazione\n");
    printf("Inserisci l'intero relativo all'opzione che si vuole eseguire: ");
    scanf("%d", &scelta);
    return scelta;
}

link carica_da_file(link h){
    FILE *fp;
    item_t tmp;
    char nome_file[MAXLEN];

    printf("Inserisci il nome del file: ");
    scanf("%s", nome_file);

    fp = fopen(nome_file, "r");
    if (fp == NULL){
        printf("File non trovato;");
        return h;
    }

    while(fscanf(fp, "%s %s %s %d/%d/%d %s %s %d" , tmp.codice, tmp.nome, tmp.cognome, &tmp.data_di_nascita.giorno, &tmp.data_di_nascita.mese, &tmp.data_di_nascita.anno, tmp.via, tmp.citta, &tmp.cap) != EOF)
        h = SortListIns(h, tmp);
    printf("caricamento da file effettuato;\n");
    return h;
}

link SortListIns(link h, item_t val){
    link x, p;
    if( h==NULL || confronta_data(h->val.data_di_nascita, val.data_di_nascita))
        return newNode(val, h);
    for(x=h->next, p=h; x!=NULL && confronta_data(val.data_di_nascita,x->val.data_di_nascita); p=x, x=x->next);
    p->next = newNode(val, x);
    return h;
}

int confronta_data(data_t data1, data_t data2){
    if((data1.anno > data2.anno) || (data1.anno==data2.anno && data1.mese>data2.mese) || (data1.anno==data2.anno && data1.mese==data2.mese && data1.giorno>data2.giorno))
        return 1; //data1 > data2
    return 0; //data2 > data1
}

link newNode(item_t val, link next){
    link x = malloc(sizeof *x);
    if(x == NULL)
        return NULL;
    else{
        x->val = val;
        x->next = next;
    }
    return x;
}

link inserisci_da_tastiera(link h){
    item_t tmp;
    printf("Inserisci i dettagli dell'anagrafica nella forma ");
    printf("<codice> <nome> <cognome> <data_di_nascita> <via> <citta'> <cap>: \n");
    scanf("%s %s %s %d/%d/%d %s %s %d" , tmp.codice, tmp.nome, tmp.cognome, &tmp.data_di_nascita.giorno, &tmp.data_di_nascita.mese, &tmp.data_di_nascita.anno, tmp.via, tmp.citta, &tmp.cap);
    h = SortListIns(h, tmp);
    printf("L'elemento e' stato inserito in lista!\n");
    return h;
}

item_t ITEMsetvoid(){
    item_t i;
    i.cap = -1;
    return i;
}


void stampa_dato(item_t i, int mess_finale, FILE *fp){
    if(is_void(i)){
        if(mess_finale==0)
            printf("Anagrafica non trovata;\n");
        else
            printf("Fine;\n");
        return;
    }
    if(fp==NULL)
        printf("%s %s %s %d/%d/%d %s %s %d \n", i.codice, i.nome, i.cognome, i.data_di_nascita.giorno, i.data_di_nascita.mese, i.data_di_nascita.anno, i.via, i.citta, i.cap);
    else
        fprintf(fp, "%s %s %s %d/%d/%d %s %s %d \n", i.codice, i.nome, i.cognome, i.data_di_nascita.giorno, i.data_di_nascita.mese, i.data_di_nascita.anno, i.via, i.citta, i.cap);
}

int is_void(item_t i){
    if(i.cap == -1)
        return 1;
    return 0;
}