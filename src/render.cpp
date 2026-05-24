# include "raylib.h"
# include "render.hpp"
# include "pieces.hpp"
# include "core.hpp"

void DrawTextCentered(const char* text, Rectangle btn, int fontSize, Color color){
    int textWidth = MeasureText(text, fontSize);
    int textX = btn.x + (btn.width - textWidth) / 2;
    int textY = btn.y + (btn.height - fontSize) / 2;
    DrawText(text, textX, textY, fontSize, color);
}

void DrawTitleCentered(const char* text, int y, int fontSize, Color color){
    int textWidth = MeasureText(text, fontSize);
    int textX = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, textX, y, fontSize, color);
}

Menu update_menu(Menu menu_state, struct StatesVariables *states, int score_position){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const float btnWidth = 260;
    const float btnHeight = 50;
    const float spacing = 25;
    const float btnX = (screenWidth - btnWidth) / 2;
    const float startY = 280;    
    const float pausedY = 450;

    const int board_widthRL = BOARD_WIDTH * CELL_SIZE, board_heightRL = (BOARD_HEIGHT - 2) * CELL_SIZE;
    const int offset_x = (screenWidth - board_widthRL) >> 1, offset_y = (screenHeight - board_heightRL) >> 1; 

    Vector2 mouse = GetMousePosition(); // Mouse input.

    Rectangle btn_play = {btnX, startY, btnWidth, btnHeight};
    Rectangle btn_level = {btnX, startY + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit = {btnX, startY + (btnHeight + spacing) * 2, btnWidth, btnHeight};
    Rectangle btn_instructions = {930, 20, 50, 50};
    Rectangle btn_instructions_back = {930, 20, 50, 50};

    Rectangle btn_resume = {btnX, pausedY, btnWidth, btnHeight};
    Rectangle btn_restart = {btnX, pausedY + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit_menu = {btnX, pausedY + (btnHeight + spacing) * 2, btnWidth, btnHeight};

    switch(menu_state){
        case MAIN_MENU:{ // ---------------- MAIN MENU ----------------
            DrawTitleCentered("Tetrix", 80, 140, WHITE);
            DrawRectangle((screenWidth - 300) / 2, 630, 300, 280, (Color){200, 200, 200, 150});
            draw_scores(states->best_user, states->best_scores); 
            float centerX_menu_princ = (GetScreenWidth() - btn_play.width) / 2;
            btn_play.x = centerX_menu_princ;
            btn_level.x = centerX_menu_princ;
            btn_exit.x = centerX_menu_princ;

            if(CheckCollisionPointRec(mouse, btn_play)){ // --- PLAY ---
                DrawRectangleRec(btn_play, DARKGRAY);
                DrawTextCentered("PLAY", btn_play, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = RESUME; }
            } else { 
                DrawRectangleRec(btn_play, (Color){200, 200, 200, 150});
                DrawTextCentered("PLAY", btn_play, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_level)){ // --- CHOOSE LEVEL ---
                DrawRectangleRec(btn_level, DARKGRAY);
                DrawTextCentered(TextFormat("LEVEL %d", states->choosed_level), btn_level, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = LEVEL;}
            } else {
                DrawRectangleRec(btn_level, (Color){200, 200, 200, 150});
                DrawTextCentered(TextFormat("LEVEL %d", states->choosed_level), btn_level, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_exit)){ // --- EXIT ---
                DrawRectangleRec(btn_exit, DARKGRAY);
                DrawTextCentered("EXIT", btn_exit, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ states->exit_raylib_window = true; }
            } else {
                DrawRectangleRec(btn_exit, (Color){200, 200, 200, 150});
                DrawTextCentered("EXIT", btn_exit, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_instructions)){ // --- INSTRUCTIONS ---
                DrawRectangleRec(btn_instructions, DARKGRAY);
                DrawTextCentered("?", btn_instructions, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = INSTRUCTION; }
            } else {
                DrawRectangleRec(btn_instructions, (Color){200, 200, 200, 150});
                DrawTextCentered("?", btn_instructions, 20, BLACK);
            }
        } break;

        case PAUSED_MENU:{ // ---------------- PLAY MENU ----------------
            float centerX = (GetScreenWidth() - btn_resume.width) / 2;
            btn_resume.x = centerX;
            btn_restart.x = centerX;
            btn_exit_menu.x = centerX;

            DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
            DrawTitleCentered("PAUSED", 350, 50, WHITE);

            if(CheckCollisionPointRec(mouse, btn_resume)){ // --- RESUME ----
                DrawRectangleRec(btn_resume, DARKGRAY);
                DrawTextCentered("RESUME", btn_resume, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ states->paused = false; menu_state = RESUME; }
            } else {
                DrawRectangleRec(btn_resume, (Color){200, 200, 200, 150});
                DrawTextCentered("RESUME", btn_resume, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_restart)){ // --- RESTART ----
                DrawRectangleRec(btn_restart, DARKGRAY);
                DrawTextCentered("RESTART", btn_restart, 20, WHITE);
                // if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ states->paused = false; menu_state = RESUME; }
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ states->restart = true; }
            } else {
                DrawRectangleRec(btn_restart, (Color){200, 200, 200, 150});
                DrawTextCentered("RESTART", btn_restart, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_exit_menu)){ // --- RETURN TO MENU ----
                DrawRectangleRec(btn_exit_menu, DARKGRAY);
                DrawTextCentered("RETURN TO MENU", btn_exit_menu, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = MAIN_MENU; }
            } else {
                DrawRectangleRec(btn_exit_menu, (Color){200, 200, 200, 150});
                DrawTextCentered("RETURN TO MENU", btn_exit_menu, 20, BLACK);
            }
        } break;

        case INSTRUCTION:{
            if(CheckCollisionPointRec(mouse, btn_instructions_back)){ // --- INSTRUCTIONS BACK ---
                DrawRectangleRec(btn_instructions_back, DARKGRAY);
                DrawTextCentered("<-", btn_instructions_back, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = MAIN_MENU; }
            } else {
                DrawRectangleRec(btn_instructions_back, (Color){200, 200, 200, 150});
                DrawTextCentered("<-", btn_instructions_back, 20, BLACK);
            }
        } break;

        case GAME_OVER:{ // ---------------- GAME OVER ----------------
            DrawRectangle(offset_x, offset_y, board_widthRL, board_heightRL, Fade(BLACK, 0.8f)); 
            DrawTitleCentered("GAME OVER", ((screenHeight - 50) >> 1) - 200, 50, RED);

            int box_width = 220, box_height = 60;
            int box_x = (screenWidth - box_width) / 2, box_y = 520;

            if(score_position >= 0 && score_position < 5){
                DrawTitleCentered("NEW HIGH SCORE!!", ((screenHeight - 50) >> 1) - 100, 50, GOLD);
                DrawRectangle(box_x, box_y, box_width, box_height, RAYWHITE);
                DrawRectangleLines(box_x, box_y, box_width, box_height, GOLD);

                const char* name = states->best_user[score_position];

                int text_width = MeasureText(name, 35);
                int text_x = box_x + (box_width - text_width) / 2;
                int text_y = box_y + (box_height -35) / 2;
                DrawText(name, text_x, text_y, 35, BLACK);
            }
            DrawTitleCentered("PRESS ENTER TO MAIN MENU", 930, 30, WHITE);
        } break;
    }
    return menu_state;
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
    int screen_width = 1000, screen_height = 1000;

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

    DrawRectangle(45, 680, 245, 180, BLACK);
    DrawText(TextFormat("Score: %lu", states->score), (screen_width / 4) - 170, 705, 25, WHITE); 
    DrawText(TextFormat("Level: %lu", states->level), (screen_width / 4) - 170, 745, 25, WHITE); 
    DrawText(TextFormat("Eliminated : %lu", states->eliminated_lines), (screen_width / 4) - 170, 785, 25, WHITE); 
    DrawText("lines", (screen_width / 4) - 150, 810, 25, WHITE);
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

void draw_scores(char best_user[][4], long int best_scores[]){
    DrawTitleCentered("HIGH SCORES:", 660, 30, BLACK);
    for(int i = 0; i < 5; i++){
        DrawText(
            TextFormat("%s\t%ld", best_user[i], best_scores[i]), 
            (GetScreenWidth() - MeasureText(TextFormat("%s\t%ld", best_user[i], best_scores[i]), 25)) >> 1, 
            700 + (i * 40), 
            25, 
            BLACK
        );
    }    
}

void draw_instructions(){
    DrawTitleCentered("Instructions", 50, 40, WHITE);

    DrawText("Movements:", 50, 100, 30, WHITE);
    DrawText("- Press the left and right arrows to lateral movements.", 70, 135, 25, WHITE);
    DrawText("- Press the up arrow to rotate the piece 90 degrees to the right.", 70, 165, 25, WHITE);
    DrawText("- Press the down arrow to drop the piece faster.", 70, 195, 25, WHITE);
    DrawText("- Press the SPACE key to drop the piece until the bottom.", 70, 225, 25, WHITE);
    DrawText("- Press the C key to save the current piece.", 70, 255, 25, WHITE);
    DrawText("- Press the Z key to rotate the piece 90 degrees to the left.", 70, 285, 25, WHITE);

    DrawText("From the main menu:", 50, 320, 30, WHITE);
    DrawText("- Press the 1 key or the PLAY button to start the game.", 70, 355, 25, WHITE);
    DrawText("- Press the 2 key or the LEVEL button to change the starting level.", 70, 385, 25, WHITE);
    DrawText("- Press the 3 key or the EXIT button to close the game.", 70, 415, 25, WHITE);

    DrawText("During gameplay press ESC key to pasue.", 50, 450, 30, WHITE);

    DrawText("From the pause menu:", 50, 490, 30, WHITE);
    DrawText("- Press the 1 key or the RESUME button to return to the game.", 70, 530, 25, WHITE);
    DrawText("- Press the 2 key or the RESTART button to restart the game.", 70, 570, 25, WHITE);
    DrawText("- Press the 3 key or the MAIN MENU button to return to the main menu.", 70, 610, 25, WHITE);

    DrawText("Press the M key to mute/unmute the music.", 50, 650, 25, WHITE);

    DrawText("Press the P key to exit the game.", 50, 690, 25, WHITE);
}