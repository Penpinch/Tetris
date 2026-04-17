# ifndef CORE
# define CORE

# include "board.hpp"
# include "current_piece.hpp"

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
} StatesVariables;

bool check_game_over(CurrentPiece *current_piece);
void clean_piece_path(StatesVariables *states, CurrentPiece *current_piece);
void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop);
void update_score(StatesVariables *states, int eliminated_lines);
void update(Board *board, CurrentPiece *current_piece, StatesVariables *states);

# endif