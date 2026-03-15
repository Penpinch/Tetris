# ifndef PIECES
# define PIECES

typedef enum{
    EMPTY, PIECE_L, PIECE_J, PIECE_O, PIECE_S, PIECE_Z, PIECE_I, PIECE_T
} Pieces;

typedef struct{
    int block[4][4];
} Blocks;

extern const int tetris_pieces[7][4][4][4];
void get_blocks(int piece_index, int rotation_index, Blocks *new_block);

# endif