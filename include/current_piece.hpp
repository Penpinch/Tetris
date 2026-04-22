# ifndef CURRENT_PIECE
# define CURRENT_PIECE

# include "board.hpp"

struct StatesVariables; // Forward Declaration.

typedef struct{
    Board *game_board;
    int current_x, current_y;
    int piece_type;
    int rotation;
} CurrentPiece;

typedef void (*KeysFunctions)(CurrentPiece*, struct StatesVariables*); // Function pointer declaration.

typedef enum{
    GAME_OVER = -1, CRASHED, WENT_DOWN
} PieceState;

void init_current_piece(Board *board, CurrentPiece *actual);
bool can_move(CurrentPiece *actual , int new_x, int new_y, int new_rotation);
void move_to_left(CurrentPiece *actual, struct StatesVariables *states);
void move_to_right(CurrentPiece *actual, struct StatesVariables *states);
void rotate_right(CurrentPiece *actual, struct StatesVariables *states);
void rotate_left(CurrentPiece *actual, struct StatesVariables *states);
int go_down(CurrentPiece *actual);

# endif