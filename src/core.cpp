# include <windows.h>
# include <conio.h>
# include <stdio.h>

# include <time.h>
# include "core.hpp"
# include "bag_random.hpp"
# include "current_piece.hpp"
# include "board.hpp"
# include "lines.hpp"
# include "pieces.hpp"

//input(), reset(), game_over()
// nivel / velocidad
// spawn de piezas
// limpieza de líneas

void update_score(StatesVariables states, int eliminated_lines){
    switch(eliminated_lines){
        case 0: eliminated_lines = 0; break;
        case 1: eliminated_lines = SINGLE_; break;
        case 2: eliminated_lines = DOUBLE_; break;
        case 3: eliminated_lines = TRIPLE_; break;
        case 4: eliminated_lines = TETRIS; break;
    }
    states.score += eliminated_lines * (states.level + 1);
}

void clean_piece_path(StatesVariables *states, CurrentPiece *current_piece){
    Blocks piece_shape;
    get_blocks(current_piece->piece_type, current_piece->rotation, &piece_shape);
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            if(piece_shape.block[y][x] == 1){
                set_block(states->board, current_piece->current_x + x, current_piece->current_y + y, EMPTY);
            }
        }
    }
}

void gravity(StatesVariables *states, CurrentPiece *current_piece){
    static clock_t start = clock();
    clock_t end;
    bool moved = false;
    float time_difference = 0;

    end = clock();
    time_difference = ((float)(end - start)) / CLOCKS_PER_SEC;
    
    if(time_difference > states->gravity_time){
        clean_piece_path(states, current_piece);
        moved = go_down(current_piece);
        start = clock();
    }

    // if(moved){ 
    //     set_piece(
    //         states->board, current_piece->current_x, current_piece->current_y, 
    //         current_piece->piece_type, current_piece->rotation); 
    // }
}

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    bool moved = false;
    int key;
    
    key = _getch(); // Leer el primer código
    if(key == 224){ // 224 indica tecla especial(flechas)
        key = _getch(); // Leer el segundo código
        set_piece(
            states->board, current_piece->current_x, current_piece->current_y, 
            current_piece->piece_type, current_piece->rotation);

        switch(key){
            case 72: //Arriba
                clean_piece_path(states, current_piece); rotate(current_piece);
                break;
            case 80: //Abajo
                clean_piece_path(states, current_piece); moved = go_down(current_piece); 
                break;  
            case 75: //Izquierda
                clean_piece_path(states, current_piece); move_to_left(current_piece); 
                break;
            case 77: //Derecha
                clean_piece_path(states, current_piece); move_to_right(current_piece); 
                break;
        }
    }

    gravity(states, current_piece);
    states->eliminated_lines = check_lines(board);
}

// set_piece(board, 19, -1, PIECE_L, 3);
// set_piece(board, 19, 7, PIECE_J, 0);
// set_piece(board, 18, 4, PIECE_O, 0);
// set_piece(board, 19, 5, PIECE_S, 1);
// set_piece(board, 19, 2, PIECE_Z, 1);
// set_piece(board, 20, 0, PIECE_I, 1);
// set_piece(board, 17, 6, PIECE_T, 1);

// void update(){
    //     // gravedad (la pieza baja)
    //     // fijar pieza si colisiona
    //     // limpiar líneas
    //     // generar nueva pieza
    //     // detectar game over
    //     // actualizar score
// }