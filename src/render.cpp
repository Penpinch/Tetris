# include "raylib.h"
# include "render.hpp"
# include "pieces.hpp"
# include "core.hpp"

// Definimos los estados posibles de la aplicación

// Función auxiliar para dibujar texto centrado en un botón
void DrawTextCentered(const char* text, Rectangle btn, int fontSize, Color color){
    int textWidth = MeasureText(text, fontSize); //Se utiliza la función MeasureText() de Raylib. Esto mide cuántos píxeles ocupa el texto y permite colocarlo exactamente en el centro del botón, sin importar lo que escribas.
    int textX = btn.x + (btn.width - textWidth) / 2; //En lugar de coordenadas fijas, ahora se usa el ancho de la ventana 
    int textY = btn.y + (btn.height - fontSize) / 2; //para calcular el centro exacto: (screenWidth - buttonWidth) / 2.
    DrawText(text, textX, textY, fontSize, color);
}

// Función auxiliar para dibujar texto centrado en la pantalla 
void DrawTitleCentered(const char* text, int y, int fontSize, Color color){
    int textWidth = MeasureText(text, fontSize); //Se utiliza la función MeasureText() de Raylib. Esto mide cuántos píxeles ocupa el texto y permite colocarlo exactamente en el centro del botón, sin importar lo que escribas.
    int textX = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, textX, y, fontSize, color);
}

Menu update_menu(Menu menu_state, struct StatesVariables *states){
    // --- CONFIGURACIÓN DE TAMAÑOS ---
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    int btnWidth = 260;
    int btnHeight = 50;
    int spacing = 20;
    float btnX = (float)(screenWidth - btnWidth) / 2;
    float startY = 180;

    Vector2 mouse = GetMousePosition(); //Entrada del ratón.

    Rectangle btn_play   = {btnX, startY, btnWidth, btnHeight};
    Rectangle btn_level = {btnX, startY + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit  = {btnX, startY + (btnHeight + spacing) * 2, btnWidth, btnHeight};

    Rectangle btn_resume = {btnX, 150, btnWidth, btnHeight};
    Rectangle btn_restart = {btnX, 150 + (btnHeight + spacing), btnWidth, btnHeight};
    Rectangle btn_exit_menu = {btnX, 150 + (btnHeight + spacing) * 2, btnWidth, btnHeight};

    ClearBackground(RAYWHITE);

    switch(menu_state){

        // ---------------- MAIN MENU ----------------
        case MAIN_MENU:{
            ClearBackground(BEIGE);
            DrawTitleCentered("Tetrix.", 80, 40, BLACK);
            float centerX_menu_princ = (GetScreenWidth() - btn_play.width) / 2; //Centrar botones.
            btn_play.x = centerX_menu_princ;
            btn_level.x = centerX_menu_princ;
            btn_exit.x = centerX_menu_princ;

            // --- PLAY ---
            if (CheckCollisionPointRec(mouse, btn_play)){//Detecta si el ratón. esta sobre el área el botón.
                DrawRectangleRec(btn_play, GRAY);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){//Detecta si se presiona el ratón.
                    menu_state = RESUME;//Cambia a determinada pantalla.
                }
            } else {DrawRectangleRec(btn_play, LIGHTGRAY);//Dibuja el ratón.
                DrawTextCentered("PLAY", btn_play, 20, BLACK);
            }

            // --- CHOOSE LEVEL ---
            if(CheckCollisionPointRec(mouse, btn_level)){
                DrawRectangleRec(btn_level, GRAY);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    menu_state = LEVEL;
                }
            } else {DrawRectangleRec(btn_level, LIGHTGRAY);
                DrawTextCentered("CHOOSE LEVEL", btn_level, 20, BLACK);
            }

            // --- EXIT ---
            if(CheckCollisionPointRec(mouse, btn_exit)){
                DrawRectangleRec(btn_exit, GRAY);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    states->exit_raylib_window = true;
                }
            } else {DrawRectangleRec(btn_exit, LIGHTGRAY);
                DrawTextCentered("EXIT", btn_exit, 20, BLACK);
            }
        }break;

        // ---------------- PLAY_MENU ----------------
        case PAUSED_MENU:{
            ClearBackground(DARKGRAY);
            DrawTitleCentered("SELECCIONA MODO DE JUEGO", 80, 30, DARKBLUE);
            float centerX = (GetScreenWidth() - btn_resume.width) / 2;
            btn_resume.x = centerX;
            btn_restart.x = centerX;
            btn_exit_menu.x = centerX;

            // --- RESUME ----
            if(CheckCollisionPointRec(mouse, btn_resume)){
                DrawRectangleRec(btn_resume, DARKGREEN);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    menu_state = RESUME;
                }
            } else {DrawRectangleRec(btn_resume, GREEN);
                DrawTextCentered("RESUME", btn_resume, 20, WHITE);
            }

            // --- RESTART ----
            if(CheckCollisionPointRec(mouse, btn_restart)){
                DrawRectangleRec(btn_restart, MAROON);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    menu_state = RESUME;
                }
            } else {DrawRectangleRec(btn_restart, RED);
                DrawTextCentered("RESTART", btn_restart, 20, WHITE);
            }

            // --- EXIT TO MENU ----
            if(CheckCollisionPointRec(mouse, btn_exit_menu)){
                DrawRectangleRec(btn_exit_menu, PURPLE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    menu_state = MAIN_MENU;
                }
            } else {DrawRectangleRec(btn_exit_menu, VIOLET);
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
    DrawText("HIGH SCORES: ", (GetScreenWidth() - MeasureText("HIGH SCORES: ", 30) >> 1), 660, 30, GOLD);
    for(int i = 0; i < 5; i++){
        DrawText(TextFormat("%ld", best_scores[i]), (GetScreenWidth() - MeasureText(TextFormat("%ld", best_scores[i]), 25)) >> 1, 700 + (i * 40), 25, GOLD);
    }    
}