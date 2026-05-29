# include <stdlib.h>
# include "board.hpp"
# include "pieces.hpp"

Board *create_board(){ // Create the board using dynamic memory.
    Board *board = (Board*)malloc(sizeof(Board));
    if(board == NULL){ return NULL; }

    board->grid = (int**)malloc(BOARD_HEIGHT * sizeof(int*));
    if(board->grid == NULL){ free(board); return NULL; }

    for(int i = 0; i < BOARD_HEIGHT; i++){
        *(board->grid + i) = (int*)calloc(BOARD_WIDTH, sizeof(int));
        if(*(board->grid + i) == NULL){
            for(int j = 0; j < i; j++){ free(*(board->grid + j)); }
            free(board->grid);
            free(board);
            return NULL;
        }
    }
    return board;
}

void destroy_board(Board *board){
    if(board == NULL){ return; }
    for(int i = 0; i < BOARD_HEIGHT; i++){ free(*(board->grid + i)); }
    free(board->grid);
    free(board);
}

void set_block(Board *board, int x, int y, int piece_type){ board->grid[y][x] = piece_type; }

void set_piece(Board *board, int piece_x, int piece_y, int piece_type, int rotation){ // Set the piece's shape on the board.
    Blocks new_block;
    get_blocks(piece_type, rotation, &new_block);

    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            if(new_block.block[y][x] == 1){
                set_block(board, piece_x + x, piece_y + y, piece_type);
            }
        }
    }
}