# include <windows.h>
# include <string.h>
# include <conio.h>
# include <stdio.h>

# include <time.h>
# include "core.hpp"
# include "bag_random.hpp"
# include "current_piece.hpp"
# include "board.hpp"
# include "lines.hpp"
# include "pieces.hpp"

// reset(), 
// nivel / velocidad

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

void hold_piece(StatesVariables *states, CurrentPiece *current_piece){
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

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    static bool up_was_pressed = false;
    static bool space_was_pressed = false;
    static bool hold_was_pressed = false;

    // -- HOLD --
    if((GetAsyncKeyState('C') & 0x8000) && hold_was_pressed == false){
        hold_piece(states, current_piece);
        hold_was_pressed = true;
    } else { hold_was_pressed = false; }

    // -- ROTATION --
    if((GetAsyncKeyState(VK_UP) & 0x8000) && up_was_pressed == false){
        rotate(current_piece); 
        up_was_pressed = true;
    } else { up_was_pressed = false; }

    // -- LATERAL MOVEMENTS --
    if(GetAsyncKeyState(VK_LEFT) & 0x8000){ move_to_left(current_piece); }
    if(GetAsyncKeyState(VK_RIGHT) & 0x8000){ move_to_right(current_piece); }

    // -- HARD DROP --
    if((GetAsyncKeyState(VK_SPACE) & 0x8000) && space_was_pressed == false){
        while((go_down(current_piece)) == true){}
        spawn_next_piece(states, current_piece);
        // if(hold_res == GAME_OVER){ states->game_over = true; }

        space_was_pressed = true;
    } else {space_was_pressed = false; }

    // -- SOFT DROP --
    bool soft_drop = GetAsyncKeyState(VK_DOWN) & 0x8000;

    gravity(states, current_piece, soft_drop);

    int eliminated_lines_single_move = check_lines(board);
    update_score(states, eliminated_lines_single_move);
    states->eliminated_lines += eliminated_lines_single_move;
}