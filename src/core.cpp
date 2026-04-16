# include <windows.h>
# include <conio.h>
# include <stdio.h>

# include <time.h>
# include "core.hpp"
# include "bag_random.hpp"
# include "current_piece.hpp"
# include "board.hpp"
# include "lines.hpp"
# include "pieces.hpp"

//input(), reset(), game_over()
// nivel / velocidad

void update_score(StatesVariables states, int eliminated_lines){
    switch(eliminated_lines){
        case 0: eliminated_lines = 0; break;
        case 1: eliminated_lines = SINGLE_; break;
        case 2: eliminated_lines = DOUBLE_; break;
        case 3: eliminated_lines = TRIPLE_; break;
        case 4: eliminated_lines = TETRIS; break;
    }
    states.score += eliminated_lines * (states.level + 1);
}

void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop){
    static DWORD start = 0;
    DWORD end = GetTickCount();    
    DWORD time_difference = end - start;

    float speed = (soft_drop) ? states->fast_gravity_time : states->gravity_time;

    if(time_difference >= (DWORD)(speed * 1000)){
        (void)go_down(current_piece); 
        start = end; 
    }
}

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    static bool up_was_pressed = false;

    if((GetAsyncKeyState(VK_UP) & 0x8000) && up_was_pressed == true){ // Up arrow.
        rotate(current_piece); 
        up_was_pressed = true;
    } else { up_was_pressed = false; }

    if(GetAsyncKeyState(VK_LEFT) & 0x8000){ move_to_left(current_piece); } // Left arrow.
    if(GetAsyncKeyState(VK_RIGHT) & 0x8000){ move_to_right(current_piece); } // Right arrow
    bool soft_drop = GetAsyncKeyState(VK_DOWN) & 0x8000; // Down arrow.

    gravity(states, current_piece, soft_drop);
    states->eliminated_lines += check_lines(board);
}