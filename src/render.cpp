# include "raylib.h"
# include "render.hpp"
# include "pieces.hpp"
# include "core.hpp"

Color get_piece_color(int piece_type){
    switch(piece_type){
        case PIECE_L: return ORANGE;
        case PIECE_J: return BLUE;
        case PIECE_O: return YELLOW;
        case PIECE_S: return DARKGREEN;
        case PIECE_Z: return RED;
        case PIECE_I: return SKYBLUE;
        case PIECE_T: return PURPLE;
        default: return DARKGRAY;
    }
}

void draw_game(Board *board, CurrentPiece *current_piece, StatesVariables *states, int offset_x, int offset_y){
    DrawRectangle(offset_x, offset_y, BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, BLACK);

    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            int type = board->grid[y][x];
            if(type != EMPTY){
                DrawRectangle(
                    (x * CELL_SIZE) + offset_x, (y * CELL_SIZE) + offset_y, 
                    CELL_SIZE - MARGIN * 2, CELL_SIZE - MARGIN * 2, get_piece_color(type)
                );
            }
        }
    }

    Blocks shape;
    get_blocks(current_piece->piece_type, current_piece->rotation, &shape);
    Color current_color = get_piece_color(current_piece->piece_type);

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(shape.block[y][x] == 1){
                int screen_x = (current_piece->current_x + x) * CELL_SIZE;
                int screen_y = (current_piece->current_y + y) * CELL_SIZE;

                DrawRectangle(
                    offset_x + screen_x + MARGIN, offset_y + screen_y + MARGIN, 
                    CELL_SIZE - MARGIN * 2, CELL_SIZE - MARGIN * 2, current_color
                );
            }
        }
    }
}