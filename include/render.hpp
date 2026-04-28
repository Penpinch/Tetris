# ifndef RENDER
# define RENDER

# include "board.hpp"
# include "current_piece.hpp"
# include "core.hpp"

constexpr int ROWS = 20;
constexpr int COLS = 10;
constexpr int CELL_SIZE = 40;
constexpr int MARGIN = 2;

Color get_piece_color(int piece_type);
void draw_game(Board *board, CurrentPiece *current_piece, StatesVariables *states, int offset_x, int offset_y);

# endif