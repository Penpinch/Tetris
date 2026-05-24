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
    StatesVariables states = {&board, 0, 0, 0, 0, 0.8, 0.05, false, false, false, EMPTY, true, h_b, EMPTY, nx_p, false, false, {0, 0, 0, 0, 0}, {"-", "-", "-", "-", "-"}};

    current_piece.piece_type = next_piece();
    current_piece.current_x = 3;
    states.next_piece_type = next_piece();
    update_difficulty(&states);
    init_keyboard(&current_piece, &states);
    read_file(states.best_user, states.best_scores);

    Menu menu_state = MAIN_MENU;

    int screen_width = 1000, screen_height = 1000;
    int board_widthRL = BOARD_WIDTH * CELL_SIZE, board_heightRL = (BOARD_HEIGHT - 2) * CELL_SIZE;
    int offset_x = (screen_width - board_widthRL) >> 1, offset_y = (screen_height - board_heightRL) >> 1; 

    InitWindow(screen_width, screen_height, "Tetris");
    SetExitKey(KEY_NINE);
    InitAudioDevice();

    Texture2D background_image;
    background_image = LoadTexture("../tetris_background.png");

    Music music = LoadMusicStream("../temporal_tetris_theme.ogg");
    PlayMusicStream(music);

    bool score_checked = false;
    bool typing_name = false;
    int score_position = -1;

    while(!WindowShouldClose() && !states.exit_raylib_window){
        UpdateMusicStream(music);
        if(IsKeyPressed(KEY_M)){ mute_unmute_music(music); }
        write_file(states.best_user, states.best_scores);

        switch(menu_state){
            case MAIN_MENU:
                read_file(states.best_user, states.best_scores);
                if(IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)){ update_difficulty(&states); menu_state = RESUME; }
                if(IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)){ update_difficulty(&states); menu_state = LEVEL; }
                if(IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)){ states.exit_raylib_window = true; }
                if(IsKeyPressed(KEY_I)){ menu_state = INSTRUCTION; }
                break;
            case LEVEL:
                update_level(&states);
                menu_state = MAIN_MENU;
                break;
            case PAUSED_MENU:
                if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)){ states.paused = false, menu_state = RESUME; }
                if(IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)){ states.restart = true; }
                if(IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)){ menu_state = MAIN_MENU; }
                break;
            case INSTRUCTION:
                if(IsKeyPressed(KEY_I)){ menu_state = MAIN_MENU; }    
                break;
            case GAME_OVER:
                if(!score_checked){
                    score_position = update_best_scores(&states);
                    if(score_position != -1){ typing_name = true; }
                    score_checked = true;
                }

                if(typing_name){
                    typing_name = capture_name(states.best_user, score_position);
                    if(!typing_name){ write_file(states.best_user, states.best_scores); }
                }

                if(!typing_name && IsKeyPressed(KEY_ENTER)){
                    score_checked = false; reset(&current_piece, &states); menu_state = MAIN_MENU; 
                }
                break;
            case RESUME:
                update(&board, &current_piece, &states);
                if(IsKeyPressed(KEY_ESCAPE)){ states.paused = true; menu_state = PAUSED_MENU; }
                if(states.game_over){ menu_state = GAME_OVER; }
                break;
        }

        if(states.restart){
            int lvl = states.choosed_level; 
            reset(&current_piece, &states); 
            states.choosed_level = lvl; 
            update_difficulty(&states);
            menu_state = RESUME; 
        }

        if(menu_state == MAIN_MENU && (states.paused || states.game_over)){
            reset(&current_piece, &states); 
        }

        BeginDrawing(); // --- RENDER --- 
        ClearBackground(BLACK);
        DrawTextureV(background_image, Vector2{0.0f, 0.0f}, Fade(WHITE, 0.9f));

        if(menu_state == RESUME || menu_state == PAUSED_MENU || menu_state == GAME_OVER){ 
            draw_game(&board, &current_piece, &states, offset_x, offset_y); 
        }

        if(menu_state == INSTRUCTION){ DrawTextureV(background_image, Vector2{0.0f, 0.0f}, Fade(WHITE, 0.5f)); draw_instructions(); }

        menu_state = update_menu(menu_state, &states, score_position);
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}