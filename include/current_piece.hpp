# ifndef CURRENT_PIECE
# define CURRENT_PIECE

# include "board.hpp"

typedef struct{
    Board *game_board;
    int current_x, current_y;
    int piece_type;
    int rotation;
} CurrentPiece;

void init_current_piece(Board *board, CurrentPiece *actual);
bool can_move(CurrentPiece *actual , int new_x, int new_y, int new_rotation);
void move_to_left(CurrentPiece *actual);
void move_to_right(CurrentPiece *actual);
void rotate(CurrentPiece *actual);
bool go_down(CurrentPiece *actual);

# endif