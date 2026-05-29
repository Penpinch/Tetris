# ifndef RENDER
# define RENDER

# include "board.hpp"
# include "current_piece.hpp"
# include "core.hpp"

constexpr int ROWS = 20; // constexpr only C++
constexpr int COLS = 10;
constexpr int CELL_SIZE = 40;
constexpr int MARGIN = 2;

struct StatesVariables; // Forward Declaration.

void DrawTextCentered(const char* text, Rectangle btn, int fontSize, Color color);
void DrawTitleCentered(const char* text, int y, int fontSize, Color color);
Menu update_menu(Menu menu_state, struct StatesVariables *states, int score_position);

Color get_piece_color(int piece_type);
int get_ghost_coord(CurrentPiece *current_piece);
void draw_ghost_piece(CurrentPiece *current_piece, int offset_x, int offset_y);
void draw_game(Board *board, CurrentPiece *current_piece, struct StatesVariables *states, int offset_x, int offset_y);
void draw_hold_piece(struct StatesVariables *states, int offset_x, int offset_y);
void draw_next_piece(struct StatesVariables *states, int offset_x, int offset_y);
void draw_scores(char best_user[][4], long int best_scores[]);
void draw_instructions();

# endif