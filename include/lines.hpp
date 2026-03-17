# ifndef LINES
# define LINES

# include "board.hpp"

bool is_full(Board *board, int row);
void remove_line(Board *board, int row); 
void move_down_all(Board *board, int eliminated_row);
int chech_lines(Board *board);

# endif