# include "current_piece.hpp"
# include "bag_random.hpp"
# include "pieces.hpp"
# include "board.hpp"
# include "lines.hpp"

void init_current_piece(Board *board, CurrentPiece *actual){
    actual->game_board = board;
    actual->piece_type = next_piece();
    actual->current_x = (BOARD_WIDTH >> 1) - 2; // Same than '/2'. Just to use Bitwise operators.
    actual->current_y = 0;
    actual->rotation = 0;
}

bool can_move(CurrentPiece *actual, int new_x, int new_y, int new_rotation){
    Blocks piece_shape;
    get_blocks(actual->piece_type, new_rotation, &piece_shape);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(piece_shape.block[i][j] == 1){
                int board_x = new_x + j;
                int board_y = new_y + i;

                if(board_x < 0 || board_x >= BOARD_WIDTH){ return false; }
                if(board_y >= BOARD_HEIGHT){ return false; }
                if(board_y >= 0 && actual->game_board->grid[board_y][board_x] != 0){ return false; }
            }
        }
    }
    return true;
}

void move_to_left(CurrentPiece *actual, struct StatesVariables *states){
    if(can_move(actual, actual->current_x - 1, actual->current_y, actual->rotation)){
        actual->current_x -= 1;
    }
}

void move_to_right(CurrentPiece *actual, struct StatesVariables *states){
    if(can_move(actual, actual->current_x + 1, actual->current_y, actual->rotation)){
        actual->current_x += 1;
    }
}

void rotate_right(CurrentPiece *actual, struct StatesVariables *states){
    int new_rotation = (actual->rotation + 1) % 4;
    if(can_move(actual, actual->current_x, actual->current_y, new_rotation)){
        actual->rotation=new_rotation;
    }
}

void rotate_left(CurrentPiece *actual, struct StatesVariables *states){
    int new_rotation = (actual->rotation + 3) % 4;
    if(can_move(actual, actual->current_x, actual->current_y, new_rotation)){
        actual->rotation = new_rotation;
    }
}

int go_down(CurrentPiece *actual){
    if(can_move(actual, actual->current_x, actual->current_y + 1, actual->rotation)){
        actual->current_y += 1;
        return WENT_DOWN;
    }
    set_piece(actual->game_board, actual->current_x, actual->current_y, actual->piece_type, actual->rotation);

    return CRASHED;
}