//g++ .\tetris.cpp .\core.cpp .\bag_random.cpp .\board.cpp .\current_piece.cpp .\lines.cpp .\pieces.cpp .\render.cpp -I..\include\ -I C:\raylib\raylib\src -L C:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -o "C:\Users\brand\OneDrive\Desktop\UAA\log programación\Tetris\output\core2.exe"

# include "raylib.h"
# include "bag_random.hpp"
# include "board.hpp"
# include "render.hpp"
# include "core.hpp"
# include "current_piece.hpp"
# include "lines.hpp"
# include "pieces.hpp"

int main(){
    Board board;
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ board.grid[a][b] = 0; } } // Initialize the board with 0.
    CurrentPiece current_piece = {&board, 0, 0, EMPTY, 0};
    Blocks h_b, nx_p; // Block for hold_block and for next_block.
    for(int i = 0; i < 4; i++){ for(int j = 0; j < 4; j++){ h_b.block[i][j] = 0; nx_p.block[i][j] = 0; } } // Init h_b, nx_p
    StatesVariables states = {&board, 0, 0, 0, 0, 0.8, 0.05, false, false,EMPTY, true, h_b, EMPTY, nx_p};

    current_piece.piece_type = next_piece();
    states.next_piece_type = next_piece();
    update_difficulty(&states);
    init_keyboard(&current_piece, &states);

    int screen_width = 600, screen_height = 1000;
    int board_widthRL = BOARD_WIDTH * CELL_SIZE, board_heightRL = BOARD_HEIGHT * CELL_SIZE;
    int offset_x = (screen_width - board_widthRL) / 2, offset_y = (screen_height - board_heightRL) / 2; 

    InitWindow(screen_width, screen_height, "Tetris");

    char game_over_text[] = "GAME OVER", paused_text[] = "PAUSED";                               // <----- Change for something better.
    int text_lenght = MeasureText(game_over_text, 50), pt_lenght = MeasureText(paused_text, 50); // <-----|

    while(!WindowShouldClose()){
        update(&board, &current_piece, &states);
        BeginDrawing();
            ClearBackground(DARKGRAY);

            if(states.paused == true){ // PAUSAR CON TECLA P
                draw_game(&board, &current_piece, &states, offset_x, offset_y);
                DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
                DrawText(paused_text, (screen_width - pt_lenght) / 2, (screen_height - 50) / 2, 50, WHITE);
            }
            else if(states.game_over == true){ 
                draw_game(&board, &current_piece, &states, offset_x, offset_y);
                DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
                DrawText(game_over_text, (screen_width - text_lenght) / 2, (screen_height - 50) / 2, 50, RED); 
            } else{ draw_game(&board, &current_piece, &states, offset_x, offset_y); }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}