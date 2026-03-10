# ifndef PIECES
# define PIECES

typedef enum{
    PIECE_L, PIECE_J, PIECE_O, PIECE_S, PIECE_Z, PIECE_I, PIECE_T
} Pieces;

extern const int tetris_pieces[7][4][4][4];

# endif