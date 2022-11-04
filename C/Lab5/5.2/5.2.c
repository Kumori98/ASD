#include <stdlib.h>
#include <stdio.h>
#define TilesFN "tiles.txt"
#define BoardFN "board.txt"


typedef struct tile{
    int rot, used;
    int val_V, val_O;    
    char tipo_V, tipo_O;
}tile_t;

typedef struct options{
    int n;
    tile_t *tiles;
}options_t;

typedef struct board{
    int R, C;
    tile_t **tiles;
}board_t;

void carica_tiles(FILE *fp, options_t *options);
void carica_board(FILE *fp, board_t *board, options_t *options);
tile_t unset();
tile_t set(int t, int r, options_t *options);




int main(){
    options_t *options;
    board_t *board;
    FILE *fp1, *fp2;
    int i;

    board = (board_t *) malloc(sizeof(board_t));
    options = (options_t *) malloc(sizeof(options_t));

    fp1 = fopen(TilesFN, "r");
    fp2 = fopen(BoardFN, "r");
    carica_tiles(fp1, options);
    carica_board(fp2, board, options);
    fclose(fp1);
    fclose(fp2);

    



    free(options->tiles);

    for(i=0; i<board->R; i++)
        free(board->tiles[i]);
    free(board->tiles);

    return 0;
}

void carica_tiles(FILE *fp, options_t *options){
    int i;
    fscanf(fp, "%d", &options->n);
    options->tiles = (tile_t *) malloc(options->n*sizeof(tile_t));
    
    for(i=0; i<options->n; i++){
        fscanf(fp, " %c %d %c %d", &options->tiles[i].tipo_V, &options->tiles[i].val_V, &options->tiles[i].tipo_O, &options->tiles[i].val_O);
        options->tiles[i].rot=-1;
        options->tiles[i].used=0;
    }
}

void carica_board(FILE *fp, board_t *board, options_t *options){
    int i, j, t, r;  

    fscanf(fp, "%d %d", &board->R, &board->C);

    board->tiles = (tile_t **) malloc(board->R*sizeof(tile_t *)); 
        for(i = 0; i<board->R; i++)
            board->tiles[i] = (tile_t *) malloc(board->C*sizeof(tile_t));
    
    for(i=0; i<board->R; i++){
        for(j=0; j<board->C; j++){
            fscanf(fp, "%d/%d", &t, &r);
            if( t==-1 || r==-1 )
                board->tiles[i][j] = unset();
            board->tiles[i][j] = set(t, r, options);
        }
    }
}

tile_t unset(){
    tile_t t;
    t.rot=0;
    t.tipo_O = '0';
    t.tipo_V = '0';
    t.val_O = -1;
    t.val_V = -1;
    t.used = 0;
    return t;
}

tile_t set(int t, int r, options_t *options){
    tile_t tile;

    tile.tipo_V = options->tiles[t].tipo_V;
    tile.tipo_O = options->tiles[t].tipo_O;
    tile.val_V = options->tiles[t].val_V;
    tile.val_O = options->tiles[t].val_O;
    tile.rot = r;
    tile.used =1;

    return tile;
}