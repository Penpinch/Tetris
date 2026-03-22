# ifndef CURRENT_PIECE
# define CURRENT_PIECE

# include "board.hpp"

typedef struct{
    Board *game_board;
    int currect_x, currect_y;
    int piece_type;
    int rotation;
} Current_piece;

void init_current_piece(Board *board, Current_piece *actual);
bool can_move(Current_piece *actual , int new_x, int new_y, int new_rotation);
void move_to_left(Current_piece *actual);
void move_to_right(Current_piece *actual);
void rotate(Current_piece *actual);
void go_down(Current_piece *actual);

# endif