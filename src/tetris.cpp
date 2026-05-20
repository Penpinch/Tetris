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
    StatesVariables states = {&board, 0, 0, 0, 0, 0.8, 0.05, false, false, EMPTY, true, h_b, EMPTY, nx_p, false, false, {0, 0, 0, 0, 0}};

    current_piece.piece_type = next_piece();
    current_piece.current_x = 3;
    states.next_piece_type = next_piece();
    update_difficulty(&states);
    init_keyboard(&current_piece, &states);
    read_file(states.best_scores);

    Menu menu_state = MAIN_MENU;

    int screen_width = 1000, screen_height = 1000;
    int board_widthRL = BOARD_WIDTH * CELL_SIZE, board_heightRL = (BOARD_HEIGHT - 2) * CELL_SIZE;
    int offset_x = (screen_width - board_widthRL) >> 1, offset_y = (screen_height - board_heightRL) >> 1; 

    InitWindow(screen_width, screen_height, "Tetris");
    SetExitKey(KEY_P);
    InitAudioDevice();
    /* -------------------------------------------------------------------------------------
    See core.cpp lines  117 - 144 to understand how this is working at this point.
    ----------------------------------------------------------------------------------------*/
    
    Texture2D background_image;
    background_image = LoadTexture("../tetris_background.png");

    Music music = LoadMusicStream("../temporal_tetris_theme.mp3");
    PlayMusicStream(music);

    while(!WindowShouldClose() && !states.exit_raylib_window){
        UpdateMusicStream(music);
        DrawTexture(background_image, 0, 0, Fade(WHITE, 0.7f));  

        write_file(states.best_scores);
        menu_state = update_menu(menu_state, &states);

        switch(menu_state){
            case MAIN_MENU:
                read_file(states.best_scores);
                if(IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)){ update_difficulty(&states); menu_state = RESUME; }
                if(IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)){ update_difficulty(&states); menu_state = LEVEL; }
                if(IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)){ states.exit_raylib_window = true; }
                break;
            case(LEVEL):
                update_level(&states);
                menu_state = MAIN_MENU;
                break;
            case PAUSED_MENU:
                if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)){ states.paused = false, menu_state = RESUME; }
                if(IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)){ 
                    int lvl = states.choosed_level; 
                    reset(&current_piece, &states); 
                    states.choosed_level = lvl; 
                    update_difficulty(&states);
                    menu_state = RESUME; }
                if(IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)){ reset(&current_piece, &states); menu_state = MAIN_MENU; }
                break;
            case GAME_OVER:
                if(IsKeyPressed(KEY_ENTER)){ reset(&current_piece, &states); menu_state = MAIN_MENU; write_file(states.best_scores); }
            case RESUME:
                update(&board, &current_piece, &states);
                if(IsKeyPressed(KEY_ESCAPE)){ states.paused = true; menu_state = PAUSED_MENU; }
                if(states.game_over){ menu_state = GAME_OVER; }
                break;
            }

            BeginDrawing(); // --- RENDER ---
            if(menu_state == MAIN_MENU || menu_state == LEVEL){ draw_scores(states.best_scores); } 
            else {
                draw_game(&board, &current_piece, &states, offset_x, offset_y); // TO DO: se queda
                DrawText(TextFormat("%lu", states.score), (screen_width / 4) - 100, 700, 25, WHITE); // Shows the score on screen

                if(menu_state == PAUSED_MENU){
                    DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
                    DrawText("PAUSED", (screen_width - MeasureText("PAUSED", 50)) >> 1, 400, 50, WHITE);

                    DrawText("1. RESUME", (screen_width - MeasureText("1. RESUME", 20)) >> 1, (screen_height >> 1) - 20, 20, GOLD);
                    DrawText("2. RESTART", (screen_width - MeasureText("2. RESTART", 20)) >> 1, (screen_height >> 1) + 20, 20, GOLD);
                    DrawText("3. MAIN MENU", (screen_width - MeasureText("3. MAIN MENU", 20)) >> 1, (screen_height >> 1) + 60, 20, GOLD);
                }
                if(menu_state == GAME_OVER){
                    update_best_scores(&states); // TO DO: se queda

                    DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
                    DrawText("GAME OVER", (screen_width - MeasureText("GAME OVER", 50)) >> 1, (screen_height - 50) >> 1, 50, RED); 
                    DrawText("Enter to main menu", (screen_width - MeasureText("Enter to main menu", 30)) >> 1, 750, 30, WHITE);
                }
            }
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}