# ifndef CURRENT_PIECE
# define CURRENT_PIECE

# include "board.hpp"

void init_current_piece(Board *board);
bool can_move(int new_x, int new_y, int new_rotation);
void go_down();
void rotate();
void move_to_left();
void move_to_right();

# endif