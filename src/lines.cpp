# include "lines.hpp"
# include "pieces.hpp"

bool is_full(Board *board, int row){ // Check if the entire line has blocks.
    for(int i = 0; i < BOARD_WIDTH; i++){ if(board->grid[row][i] == 0){ return false; } }
    return true;
}

void remove_line(Board *board, int row){
    for(int i = 0; i < BOARD_WIDTH; i++){ board->grid[row][i] = EMPTY; }
}

void move_down_all(Board *board, int eliminated_row){ // Move the entire upper structure down to the location of the deleted line.
    for(int y = eliminated_row; y > 0; y--){
        for(int x = 0; x < BOARD_WIDTH; x++){ board->grid[y][x] = board->grid[y - 1][x]; }
    }

    for(int i = 0; i < BOARD_WIDTH; i++){ board->grid[0][i] = EMPTY; }
}

int check_lines(Board *board){ // Check how many lines were deleted per action.
    int eliminated_lines = 0;
    for(int i = BOARD_HEIGHT - 1; i >= 0; i--){
        if(is_full(board, i) == true){
            remove_line(board, i);
            eliminated_lines += 1;
            move_down_all(board, i);
            i++;
        }
    }
    return eliminated_lines;
}