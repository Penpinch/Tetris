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

Menu update_menu(Menu menu_state, struct StatesVariables *states){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    float btnWidth = 260;
    float btnHeight = 50;
    float spacing = 25;
    float btnX = (screenWidth - btnWidth) / 2;
    float startY = 280;    

    Vector2 mouse = GetMousePosition(); // Mouse input.

    Rectangle btn_play = {btnX, startY, btnWidth, btnHeight};
    Rectangle btn_level = {btnX, startY + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit = {btnX, startY + (btnHeight + spacing) * 2, btnWidth, btnHeight};

    Rectangle btn_resume = {btnX, 150, btnWidth, btnHeight};
    Rectangle btn_restart = {btnX, 150 + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit_menu = {btnX, 150 + (btnHeight + spacing) * 2, btnWidth, btnHeight};

    switch(menu_state){
        case MAIN_MENU:{ // ---------------- MAIN MENU ----------------
            DrawTitleCentered("Tetrix", 80, 140, WHITE);
            DrawRectangle((screenWidth - 300) / 2, 630, 300, 280, (Color){200, 200, 200, 150});
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
                DrawTextCentered(TextFormat("Level %d", states->choosed_level), btn_level, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = LEVEL;}
            } else {
                DrawRectangleRec(btn_level, (Color){200, 200, 200, 150});
                DrawTextCentered(TextFormat("Level %d", states->choosed_level), btn_level, 20, BLACK);
            }

            if(CheckCollisionPointRec(mouse, btn_exit)){ // --- EXIT ---
                DrawRectangleRec(btn_exit, DARKGRAY);
                DrawTextCentered("EXIT", btn_exit, 20, WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ states->exit_raylib_window = true; }
            } else {
                DrawRectangleRec(btn_exit, (Color){200, 200, 200, 150});
                DrawTextCentered("EXIT", btn_exit, 20, BLACK);
            }
        } break;

        case PAUSED_MENU:{ // ---------------- PLAY_MENU ----------------
            // DrawTitleCentered("SELECCIONA MODO DE JUEGO", 80, 30, DARKBLUE);
            float centerX = (GetScreenWidth() - btn_resume.width) / 2;
            btn_resume.x = centerX;
            btn_restart.x = centerX;
            btn_exit_menu.x = centerX;

            if(CheckCollisionPointRec(mouse, btn_resume)){ // --- RESUME ----
                DrawRectangleRec(btn_resume, DARKGREEN);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = RESUME; }
            } else {
                DrawRectangleRec(btn_resume, GREEN);
                DrawTextCentered("RESUME", btn_resume, 20, WHITE);
            }

            if(CheckCollisionPointRec(mouse, btn_restart)){ // --- RESTART ----
                DrawRectangleRec(btn_restart, MAROON);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = RESUME; }
            } else {
                DrawRectangleRec(btn_restart, RED);
                DrawTextCentered("RESTART", btn_restart, 20, WHITE);
            }

            if(CheckCollisionPointRec(mouse, btn_exit_menu)){ // --- EXIT TO MENU ----
                DrawRectangleRec(btn_exit_menu, PURPLE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ menu_state = MAIN_MENU; }
            } else {
                DrawRectangleRec(btn_exit_menu, VIOLET);
                DrawTextCentered("RETURN TO MENU", btn_exit_menu, 20, WHITE);
            }
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
    DrawText("HIGH SCORES: ", (GetScreenWidth() - MeasureText("HIGH SCORES: ", 30) >> 1), 660, 30, BLACK);
    for(int i = 0; i < 5; i++){
        DrawText(TextFormat("%ld", best_scores[i]), (GetScreenWidth() - MeasureText(TextFormat("%ld", best_scores[i]), 25)) >> 1, 700 + (i * 40), 25, BLACK);
    }    
}