# ifndef CORE
# define CORE

# include "board.hpp"
# include "current_piece.hpp"
# include "pieces.hpp"
// typedef enum{
//     PLAYING, PAUSED, GAME_OVER
// } GameStates;

typedef enum{
    SINGLE_ = 100, DOUBLE_ = 300, TRIPLE_ = 500, TETRIS = 800
} BaseScore;

typedef struct{
    Board *board;
    unsigned long int score;
    int level;
    int eliminated_lines;
    float gravity_time;
    float fast_gravity_time;
    bool game_over;
    int hold_piece_type;
    bool can_be_holded;
    Blocks hold_block;
    int next_piece_type;
    Blocks next_block;
} StatesVariables;

void update_score(StatesVariables *states, int eliminated_lines);
void spawn_next_piece(StatesVariables *states, CurrentPiece *current_piece);
void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop);
void hold_piece(StatesVariables *states, CurrentPiece *current_piece);
void update(Board *board, CurrentPiece *current_piece, StatesVariables *states);

# endif