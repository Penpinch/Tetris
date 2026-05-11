# include "raylib.h"
# include "render.hpp"
# include "pieces.hpp"
# include "core.hpp"

void temporal_pause_screen(struct StatesVariables *states, int *ch){
    BeginDrawing();
    ClearBackground(PURPLE);
    if(states->paused){
        if(IsKeyPressed(KEY_ONE)){ *ch = 1; DrawText("1 pasued pressed", 500, 500, 50, GOLD); }
        else if(IsKeyPressed(KEY_TWO)){ *ch = 2; DrawText("2 paused pressed", 500, 500, 50, GOLD); }
        else if(IsKeyPressed(KEY_THREE)){ *ch = 3; DrawText("3 paused pressed", 500, 500, 50, GOLD); }
    } else {
        if(IsKeyPressed(KEY_ONE)){ *ch = 1; DrawText("1 main pressed", 500, 500, 50, GOLD); }
        else if(IsKeyPressed(KEY_TWO)){ *ch = 2; DrawText("2 main pressed", 500, 500, 50, GOLD); }
        else if(IsKeyPressed(KEY_THREE)){ *ch = 3; DrawText("3 main pressed", 500, 500, 50, GOLD); }
    }
    EndDrawing();
}

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

void draw_game(Board *board, CurrentPiece *current_piece, struct StatesVariables *states, int offset_x, int offset_y){
    DrawRectangle(offset_x, offset_y, BOARD_WIDTH * CELL_SIZE, (BOARD_HEIGHT - 2) * CELL_SIZE, BLACK);

    for(int y = 2; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            int type = board->grid[y][x];
            if(type != EMPTY){
                DrawRectangle(
                    (x * CELL_SIZE) + offset_x + MARGIN, ((y - 2) * CELL_SIZE) + offset_y + MARGIN, 
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
                int board_y = current_piece->current_y + y;
                
                if(board_y >= 2){
                    int screen_x = (current_piece->current_x + x) * CELL_SIZE;
                    int screen_y = (board_y - 2) * CELL_SIZE;
    
                    DrawRectangle(
                        offset_x + screen_x + MARGIN, offset_y + screen_y + MARGIN, 
                        CELL_SIZE - MARGIN * 2, CELL_SIZE - MARGIN * 2, current_color
                    );
                }
            }
        }
    }

    draw_hold_piece(states, offset_x, offset_y);
    draw_next_piece(states, offset_x, offset_y);
}

void draw_hold_piece(struct StatesVariables *states, int offset_x, int offset_y){
    int hold_x = offset_x - (4 * CELL_SIZE) - 20, hold_y = offset_y + (3 * CELL_SIZE);
    DrawRectangle(hold_x, hold_y, 4 * CELL_SIZE + 10, 4 * CELL_SIZE + 10, BLACK);

    if(states->hold_piece_type == EMPTY){ return; }

    Blocks shape;
    get_blocks(states->hold_piece_type, 0, &shape);
    Color current_color = get_piece_color(states->hold_piece_type);

    DrawRectangle(hold_x, hold_y, 4 * CELL_SIZE + 10, 4 * CELL_SIZE + 10, BLACK);
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(shape.block[y][x] == 1){
                int screen_x = x * CELL_SIZE;
                int screen_y = y * CELL_SIZE;

                DrawRectangle(
                    hold_x + screen_x + MARGIN + 5, hold_y + screen_y + MARGIN + 5, 
                    CELL_SIZE - MARGIN * 2, CELL_SIZE - MARGIN * 2, current_color
                );
            }
        }
    }
}

void draw_next_piece(struct StatesVariables *states, int offset_x, int offset_y){
    int next_x = offset_x + (BOARD_WIDTH * CELL_SIZE) + 10, next_y = offset_y + (3 * CELL_SIZE);
    DrawRectangle(next_x, next_y, 4 * CELL_SIZE + 10, 4 * CELL_SIZE + 10, BLACK);

    if(states->next_piece_type == EMPTY){ return; }

    Blocks shape;
    get_blocks(states->next_piece_type, 0, &shape);
    Color current_color = get_piece_color(states->next_piece_type);

    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(shape.block[y][x] == 1){
                int screen_x = x * CELL_SIZE;
                int screen_y = y * CELL_SIZE;

                DrawRectangle(
                    next_x + screen_x + MARGIN + 5, next_y + screen_y + MARGIN + 5, 
                    CELL_SIZE - MARGIN * 2, CELL_SIZE - MARGIN * 2, current_color
                );
            }
        }
    }
}

void draw_scores(long int best_scores[]){
    DrawText("HIGH SCORES: ", (GetScreenWidth() - MeasureText("HIGH SCORES: ", 30) >> 1), 660, 30, GOLD);
    for(int i = 0; i < 5; i++){
        DrawText(TextFormat("%ld", best_scores[i]), (GetScreenWidth() - MeasureText(TextFormat("%ld", best_scores[i]), 25)) >> 1, 700 + (i * 40), 25, GOLD);
    }    
}