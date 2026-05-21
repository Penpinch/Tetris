# ifndef CORE
# define CORE

# include "board.hpp"
# include "current_piece.hpp"
# include "pieces.hpp"

typedef enum{
    SINGLE_ = 100, DOUBLE_ = 300, TRIPLE_ = 500, TETRIS = 800
} BaseScore;

typedef enum{
    MAIN_MENU, PAUSED_MENU, GAME_OVER, RESUME, LEVEL, INSTRUCTION
} Menu; 

typedef struct {
    bool soft_drop;
}InputState;

typedef struct StatesVariables{
    Board *board;
    long int score;
    int choosed_level;
    int level;
    int eliminated_lines;
    float gravity_time;
    float fast_gravity_time;
    bool game_over;
    bool paused;
    int hold_piece_type;
    bool can_be_held;
    Blocks hold_block;
    int next_piece_type;
    Blocks next_block;
    bool new_game; // New game since a pause.
    bool exit_raylib_window;
    long int best_scores[5];
} StatesVariables;

void write_file(long int best_scores[]);
void read_file(long int read_scores[]);
void update_best_scores(StatesVariables *states);
void update_score(StatesVariables *states, int eliminated_lines);
void update_difficulty(StatesVariables *states);
void spawn_next_piece(StatesVariables *states, CurrentPiece *current_piece);
void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop);
void hold_piece(CurrentPiece *current_piece, StatesVariables *states);
void hard_drop(CurrentPiece *current_piece, StatesVariables *states);
void mute_unmute_music(Music music);
void pause(CurrentPiece *current_piece, StatesVariables *states);
void reset(CurrentPiece *current_piece, StatesVariables *states);
void init_keyboard(CurrentPiece *current_piece, StatesVariables *states);
void input(CurrentPiece *current_piece, StatesVariables *states, InputState *input_state);
void update_level(StatesVariables *states);
void update(Board *board, CurrentPiece *current_piece, StatesVariables *states);

# endif