# include <windows.h>
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

// reset(), 

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
    static DWORD start = 0;
    DWORD end = GetTickCount();    
    DWORD time_difference = end - start;

    float speed = (soft_drop) ? states->fast_gravity_time : states->gravity_time;

    if(time_difference >= (DWORD)(speed * 1000)){
        int go_down_result = go_down(current_piece); 
        if(go_down_result <= 0){
            spawn_next_piece(states, current_piece);
        }
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

void init_keyboard(CurrentPiece *current_piece, StatesVariables *states){ // Just to practice function pointers.
    key_actions[VK_UP] = rotate_right; // ROTATION TO THE RIGHT
    key_actions[VK_LEFT] = move_to_left; // LEFT
    key_actions[VK_RIGHT] = move_to_right; // RIGHT
    key_actions[VK_SPACE] = hard_drop; // HARD DROP
    key_actions['c'] = hold_piece; //HOLD
    key_actions['C'] = hold_piece; //HOLD
    key_actions['z'] = rotate_left; // ROTATION TO THE LEFT
    key_actions['Z'] = rotate_left; // ROTATION TO THE LEFT
}

void input(CurrentPiece *current_piece, StatesVariables *states, InputState *input_state){ // Made with a lot of AI guidance.
    static bool key_was_down[256] = {false};
    int keys[] = {'C', 'Z', VK_UP, VK_LEFT, VK_RIGHT, VK_SPACE};
    int active_keys = sizeof(keys) / sizeof(keys[0]);

    for(int i = 0; i < active_keys; i++){
        int vk = keys[i];
        bool is_down = (GetAsyncKeyState(vk) & 0x8000);

        if(is_down == true){
            if(vk == VK_LEFT || vk == VK_RIGHT){ key_actions[vk](current_piece, states); }
            else if(key_was_down[vk] == false){ key_actions[vk](current_piece, states); }
        }
        key_was_down[vk] = is_down;
    }
    input_state->soft_drop = GetAsyncKeyState(VK_DOWN) & 0x8000;
}

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    InputState input_state; // SOFT DROP
    input(current_piece, states, &input_state);

    gravity(states, current_piece, input_state.soft_drop);

    int eliminated_lines_single_move = check_lines(board);
    update_score(states, eliminated_lines_single_move);
    states->eliminated_lines += eliminated_lines_single_move;

    update_difficulty(states);
}