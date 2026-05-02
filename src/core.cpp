# include "raylib.h"
# include <math.h>
# include <string.h>
# include <conio.h>
# include <stdio.h>

# include <time.h>
# include "current_piece.hpp"
# include "core.hpp"
# include "bag_random.hpp"
# include "board.hpp"
# include "lines.hpp"
# include "pieces.hpp"

KeysFunctions key_actions[256] = {NULL}; // Functon pointer.

void update_score(StatesVariables *states, int eliminated_lines){
    switch(eliminated_lines){
        case 0: eliminated_lines = 0; break;
        case 1: eliminated_lines = SINGLE_; break;
        case 2: eliminated_lines = DOUBLE_; break;
        case 3: eliminated_lines = TRIPLE_; break;
        case 4: eliminated_lines = TETRIS; break;
    }
    states->score += eliminated_lines * (states->level + 1);
}

void update_difficulty(StatesVariables *states){
    states->level = states->choosed_level + (states->eliminated_lines / 10);
    float delay = 0.8f * pow(0.9f, states->level - 1);

    if(delay < 0.1f){ states->gravity_time = 0.1f; }
    else { states->gravity_time = delay; }
}

void spawn_next_piece(StatesVariables *states, CurrentPiece *current_piece){
    current_piece->piece_type = states->next_piece_type;
    states->next_piece_type = next_piece();
    memcpy(states->next_block.block, tetris_pieces[states->next_piece_type - 1][0], sizeof(int) * 16);

    current_piece->current_x = (BOARD_WIDTH >> 1) - 2; // Same than '/2'. Just to use Bitwise operators.
    current_piece->current_y = 0;
    current_piece->rotation = 0;
    states->can_be_holded = true;

    if(can_move(current_piece, current_piece->current_x, current_piece->current_y, current_piece->rotation) == false){
        states->game_over = true;
    }
}

void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop){
    static float start = 0;
    float end = GetTime();    
    float time_difference = end - start;

    float speed = (soft_drop) ? states->fast_gravity_time : states->gravity_time;

    if(time_difference >= (float)speed){
        int go_down_result = go_down(current_piece); 
        if(go_down_result <= 0){ spawn_next_piece(states, current_piece); }
        start = end; 
    }
}

void hold_piece(CurrentPiece *current_piece, StatesVariables *states){
    if(states->can_be_holded == false){ return; }
    if(states->hold_piece_type == EMPTY){
        states->hold_piece_type = current_piece->piece_type;
        memcpy(states->hold_block.block, tetris_pieces[states->hold_piece_type - 1][0], sizeof(int)*16);
        current_piece->piece_type = next_piece();
    } else {
        int temp = current_piece->piece_type;
        current_piece->piece_type = states->hold_piece_type;
        states->hold_piece_type = temp;
        memcpy(states->hold_block.block, tetris_pieces[temp - 1][0], sizeof(int)*16);
    }
    current_piece->current_x = (BOARD_WIDTH >> 1) - 2;
    current_piece->current_y = 0;
    current_piece->rotation = 0;
    states->can_be_holded = false;
}

void hard_drop(CurrentPiece *current_piece, StatesVariables *states){
    while((go_down(current_piece)) == 1){}
    spawn_next_piece(states, current_piece);
}

void pause(CurrentPiece *current_piece, StatesVariables *states){ 
    if(states->paused == false){ states->paused = true; }
    else{ states->paused = false; }
}

void reset(CurrentPiece *current_piece, StatesVariables *states){
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ states->board->grid[a][b] = 0; } } // Reset board.

    current_piece->piece_type = next_piece();
    current_piece->current_x = 3;
    current_piece->current_y = 0;
    current_piece->rotation = 0;

    states->score = 0;
    states->choosed_level = 0;
    states->level = 0;
    states->eliminated_lines = 0;
    states->gravity_time = 0.8;
    states->fast_gravity_time = 0.05;
    states->game_over = false;
    states->paused = false;
    states->hold_piece_type = EMPTY; 
    states->can_be_holded = true; 
    for(int i = 0; i < 4; i++){ for(int j = 0; j < 4; j++){ states->hold_block.block[i][j] = 0; } } // Reset hold_block.
    states->next_piece_type = next_piece();
    get_blocks(states->next_piece_type, 0, &states->next_block);
    states->new_game = false;
}   

void paused_menu(CurrentPiece *current_piece, StatesVariables *states){
    pause(current_piece, states);
    int choice = 0; printf("choice: "); scanf("%d", &choice);
    switch(choice){
        case 1: pause(current_piece, states); break; // RESUME
        case 2: // RESTART
            if(states->choosed_level != 0){
                int ch_l = states->choosed_level; 
                reset(current_piece, states); 
                states->choosed_level = ch_l; 
            } else{
                reset(current_piece, states); 
            }
            break;
        case 3: reset(current_piece, states); states->paused = true; main_menu(current_piece, states); break; // EXIT--> GO TO MAIN MENU 
    }
}

void main_menu(CurrentPiece *current_piece, StatesVariables *states){
    int ch = 0; printf("menu choice: "); scanf("%d", &ch);
    switch(ch){
        case 1: reset(current_piece, states); break; // PLAY
        case 2: // INITIAL LEVEL
            int initial_level; printf("Initial level: "); scanf("%d", &initial_level);
            states->choosed_level = initial_level; update_difficulty(states); break;
        case 3: states->exit_raylib_window = true; break; // EXIT GAME(WINDOW)
    }
}

void init_keyboard(CurrentPiece *current_piece, StatesVariables *states){ // Just to practice function pointers.
    key_actions[KEY_UP] = rotate_right; // ROTATION TO THE RIGHT
    key_actions[KEY_SPACE] = hard_drop; // HARD DROP
    key_actions[KEY_C] = hold_piece; //HOLD
    key_actions[KEY_Z] = rotate_left; // ROTATION TO THE LEFT
}

void input(CurrentPiece *current_piece, StatesVariables *states, InputState *input_state){ // Made with a lot of AI guidance.
    static float move_timer = 0;
    float move_delay = 0.09f; // Lateral movements every 0.09 seconds.
    if(IsKeyPressed(KEY_P)){ states->exit_raylib_window = true; } // Change for a button on screen.
    if(IsKeyPressed(KEY_ESCAPE)){ paused_menu(current_piece, states); }
    if(IsKeyPressed(KEY_R) && states->paused){ 
        states->new_game = true; 
        reset(current_piece, states); 
        states->paused = false;
        return; 
    }
    if(states->paused == true || states->game_over == true){ return; }

    int keys[] = {KEY_C, KEY_Z, KEY_UP, KEY_SPACE};
    int active_keys = sizeof(keys) / sizeof(keys[0]);

    for(int i = 0; i < active_keys; i++){
        int key = keys[i];

        if(IsKeyPressed(key) == true){
            if(key_actions[key] != NULL){ key_actions[key](current_piece, states); }
        }
    }

    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)){
        move_timer += GetFrameTime();
        if(move_timer >= move_delay){
            if(IsKeyDown(KEY_LEFT)){ move_to_left(current_piece, states); }
            if(IsKeyDown(KEY_RIGHT)){ move_to_right(current_piece, states); }
            move_timer = 0;
        }
    } else{ move_timer = move_delay; }

    input_state->soft_drop = IsKeyDown(KEY_DOWN);
}

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    InputState input_state; // SOFT DROP
    input(current_piece, states, &input_state);

    if(states->paused == true || states->game_over == true){ return; }

    gravity(states, current_piece, input_state.soft_drop);

    int eliminated_lines_single_move = check_lines(board);
    update_score(states, eliminated_lines_single_move);
    states->eliminated_lines += eliminated_lines_single_move;

    update_difficulty(states);
}