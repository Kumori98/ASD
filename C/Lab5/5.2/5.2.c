#include <stdlib.h>
#include <stdio.h>
#define TilesFN "tiles.txt"
#define BoardFN "board.txt"


typedef struct tile{
    int rot, blocked;
    int val_V, val_O;    
    char tipo_V, tipo_O;
}tile_t;

typedef struct options{
    int n;
    tile_t *tiles;
}options_t;

typedef struct board{
    int R, C;
    int *tiles;
}board_t;

void carica_tiles(FILE *fp, options_t *options);
void carica_board(FILE *fp, board_t *board, options_t *options);
void mostra_best(board_t *board, options_t *options);
int calcola_best(int pos, options_t *val, int *sol, int *b_sol, int *mark, int n, int nr, int nc, int max);
int calcola_valore(int *sol, options_t *val, int nr, int nc);
void ruota(int *val_O, int *val_V, char *tipo_O, char *tipo_V);

int main(){
    options_t *options;
    board_t *board;
    FILE *fp;
    int i;

    board = (board_t *) malloc(sizeof(board_t));
    options = (options_t *) malloc(sizeof(options_t));

    fp = fopen(TilesFN, "r");
    carica_tiles(fp, options);
    fp = fopen(BoardFN, "r");
    carica_board(fp, board, options);
    fclose(fp);

    mostra_best(board, options);

    free(options->tiles);
    free(board->tiles);
    free(options);
    free(board);

    return 0;
}

void carica_tiles(FILE *fp, options_t *options){
    int i;
    fscanf(fp, "%d", &options->n);
    options->tiles = (tile_t *) malloc(options->n*sizeof(tile_t));
    
    for(i=0; i<options->n; i++){
        fscanf(fp, " %c %d %c %d", &options->tiles[i].tipo_O, &options->tiles[i].val_O, &options->tiles[i].tipo_V, &options->tiles[i].val_V);
        options->tiles[i].rot=0;
        options->tiles[i].blocked=0;
    }
}

void carica_board(FILE *fp, board_t *board, options_t *options){
    int i, j, t, r;

    fscanf(fp, "%d %d", &board->R, &board->C);

    board->tiles = (int *) malloc((board->R*board->C)*sizeof(int)); 
    
    for(i=0; i<board->R; i++){
        for(j=0; j<board->C; j++){
            fscanf(fp, "%d/%d", &t, &r);
            if( t==-1 || r==-1 )
                board->tiles[board->C*i+j] = -1;
            else{
                board->tiles[board->C*i+j] = t;
                options->tiles[t].blocked=1;
                options->tiles[t].rot=r;
                if(r==1){ //scambio i valori
                    ruota(&(options->tiles[t].val_O), &(options->tiles[t].val_V), &(options->tiles[t].tipo_O), &(options->tiles[t].tipo_V));
                }
            }
        }
    }
}

void mostra_best(board_t *board, options_t *options){
    int i, j, *sol, *b_sol, best_val=0, *mark;

    sol = (int *) malloc((board->R*board->C)*sizeof(int));
    b_sol = (int *) malloc((board->R*board->C)*sizeof(int));
    for(i=0; i<board->R; i++)
        for(j=0; j<board->C; j++){
            sol[board->C*i+j] = board->tiles[board->C*i+j];
            b_sol[board->C*i+j] = board->tiles[board->C*i+j];
        }

    mark = (int *) malloc(options->n*sizeof(int));
    for(i=0; i<options->n; i++){
        if(options->tiles[i].blocked==1)
            mark[i]=-1;
        else
            mark[i]=0;
    }

    best_val = calcola_best(0, options, sol, b_sol, mark, options->n, board->R, board->C, best_val);

    printf("La soluzione migliore ha il punteggio massimo di %d ed è: \n", best_val);
    for(i=0; i<board->R; i++){
        for(j=0; j<board->C; j++){
            printf(" (Tubo verticale t:%c, v:%d--", options->tiles[b_sol[board->C*i+j]].tipo_V, options->tiles[b_sol[board->C*i+j]].val_V);
            printf(" Tubo orizzontale t:%c, v:%d) ", options->tiles[b_sol[board->C*i+j]].tipo_O, options->tiles[b_sol[board->C*i+j]].val_O); 
        }
        printf("\n");
    }

    free(b_sol);
    free(sol);
    free(mark);
}

int calcola_best(int pos, options_t *val, int *sol, int *b_sol, int *mark, int n, int nr, int nc, int max){
    int  i, c_val, j;
    if(pos>=(nr*nc)){
        c_val = calcola_valore(sol, val, nr, nc);
        if(c_val > max){
            max = c_val;
            for(i=0; i<nr; i++)
                for(j=0; j<nc; j++)
                    b_sol[nc*i+j] = sol[nc*i+j];
        }
        return max;
    }
    for(i=0; i<n; i++){
        if(val->tiles[sol[pos]].blocked==1){
            pos++;
        }
        else if (mark[i]==0){
            mark[i] = 1; 
            sol[pos] = i;
            val->tiles[i].rot=0;
            max = calcola_best(pos+1, val, sol, b_sol, mark, n, nr, nc, max); //prima ricorsione (non ruotato)
            val->tiles[i].rot=1;
            ruota(&(val->tiles[i].val_O), &(val->tiles[i].val_V), &(val->tiles[i].tipo_O), &(val->tiles[i].tipo_V)); //ruoto i tubi
            max = calcola_best(pos+1, val, sol, b_sol, mark, n, nr, nc, max); //seconda ricorsione (ruotato)
            ruota(&(val->tiles[i].val_O), &(val->tiles[i].val_V), &(val->tiles[i].tipo_O), &(val->tiles[i].tipo_V));   //li riporto nella posizone originale
            mark[i] = 0;
        }
    }
    return max;
}

int calcola_valore(int *sol, options_t *val, int nr, int nc){
    int i, j, tot=0, sum_o=0, sum_v=0;
    char tipo_o, tipo_v;


    for(i=0; i<nr; i++){    //calcolo le somme orizzontali
            sum_o = val->tiles[sol[nc*i]].val_O;
            tipo_o = val->tiles[sol[nc*i]].tipo_O;
        for(j=1; j<nc; j++){
                if(tipo_o == val->tiles[sol[nc*i+j]].tipo_O)
                    sum_o += val->tiles[sol[nc*i+j]].val_O;
                else{
                sum_o=0;
                break;
                }
        }
        tot += sum_o;               
    }

    for(j=0; j<nc; j++){ //calcolo le somme verticali
            sum_v = val->tiles[sol[j]].val_V;
            tipo_v = val->tiles[sol[j]].tipo_V;
        for(i=1; i<nr; i++){
                if(tipo_v == val->tiles[sol[nc*i+j]].tipo_V)
                    sum_v += val->tiles[sol[nc*i+j]].val_V;
                else{
                    sum_v=0;
                    break;
                }
        }
        tot += sum_v; 
    }
    return tot;
}

void ruota(int *val_O, int *val_V, char *tipo_O, char *tipo_V){
    int val_tmp;
    char tipo_tmp;

    val_tmp = *val_O;
    *val_O = *val_V;
    *val_V = val_tmp;
    tipo_tmp = *tipo_O;
    *tipo_O = *tipo_V;
    *tipo_V = tipo_tmp;
}