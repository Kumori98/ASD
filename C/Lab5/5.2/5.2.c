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
int calcola_best(int pos, options_t *val, int *sol, int *b_sol, int *mark, int n, int nr, int nc);
int calcola_valore(int *sol, options_t val, int nr, int nc);

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
        fscanf(fp, " %c %d %c %d", &options->tiles[i].tipo_V, &options->tiles[i].val_V, &options->tiles[i].tipo_O, &options->tiles[i].val_O);
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
            board->tiles[board->C*i+j] = t;
            options->tiles[t].blocked=1;
            options->tiles[t].rot=r;
        }
    }
}

void mostra_best(board_t *board, options_t *options){
    int i, j, *sol, *b_sol, best_val, *mark;

    sol = (int *) malloc((board->R*board->C)*sizeof(int));
    for(i=0; i<board->R; i++)
        for(j=0; j<board->C; j++)
            sol[board->C*i+j] = board->tiles[board->C*i+j];

    mark = (int *) calloc(options->n,sizeof(int));

    best_val = calcola_best(0, options, sol, board->tiles, mark, options->n, board->R, board->C);

    printf("La soluzione migliore ha il punteggio massimo di %d ed è: \n", &best_val);
    for(i=0; i<board->R; i++){
        for(j=0; j<board->C; j++){
            printf(" (Tubo verticale di tipo:%d e valore:%d)", &(options->tiles[b_sol[board->C*i+j]].tipo_V), &(options->tiles[b_sol[board->C*i+j]].tipo_O)); 
        }
        printf("\n");
    }

    free(sol);
    free(mark);
}

int calcola_best(int pos, options_t *val, int *sol, int *b_sol, int *mark, int n, int nr, int nc){
    int  i, max=0, c_val;
    if(pos>=(nr*nc)){
        //calcolo del valore della soluzione ottenuta
        //se maggiore aggiorno max e b_sol
        return max;
    }
    for(i=0; i<n; i++){
        if(mark[i]==0){
            mark[i] = 1; 
            sol[pos] = i;
            val->tiles[i].rot=0;
            max = calcola_best(pos+1, val, sol, b_sol, mark, n, nr, nc); //prima ricorsione (non ruotato)
            mark[i]=2;
            val->tiles[i].rot=1;
            max = calcola_best(pos+1, val, sol, b_sol, mark, n, nr, nc); //seconda ricorsione (ruotato)
            mark[i] = 0;
        }
    }
    return max;
}

int calcola_valore(int *sol, options_t val, int nr, int nc){
    int i, j, result=0;

    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){
            
        }
    }

    return result;
}