# include "board.hpp"
# include "pieces.hpp"

void set_block(Board *board, int x, int y, int piece_type){
    board->grid[y][x] = piece_type;
}

void set_piece(Board *board, int piece_x, int piece_y, int piece_type, int rotation){
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