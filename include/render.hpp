# ifndef RENDER
# define RENDER

# include "board.hpp"
# include "current_piece.hpp"

constexpr int ROWS = 20; // Solo C++
constexpr int COLS = 10;
constexpr int CELL_SIZE = 40;
constexpr int MARGIN = 2;

struct StatesVariables; // Forward Declaration.

void temporal_pause_screen(struct StatesVariables *states, int *ch);
Color get_piece_color(int piece_type);
void draw_game(Board *board, CurrentPiece *current_piece, struct StatesVariables *states, int offset_x, int offset_y);
void draw_hold_piece(struct StatesVariables *states, int offset_x, int offset_y);
void draw_next_piece(struct StatesVariables *states, int offset_x, int offset_y);
void draw_scores(long int best_scores[]);

# endif