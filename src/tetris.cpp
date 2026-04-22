# include <iostream>
using namespace std;
# include <stdio.h>
# include <windows.h>
# include <stdlib.h>

# include "bag_random.hpp"
# include "board.hpp"
# include "core.hpp"
# include "current_piece.hpp"
# include "lines.hpp"
# include "pieces.hpp"

bool piece_occupies(CurrentPiece piece, int x, int y){ // Made with ChatGPT.
    Blocks shape;
    get_blocks(piece.piece_type, piece.rotation, &shape);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(shape.block[j][i] == 1){
                if(piece.current_x + i == x && piece.current_y + j == y){ return true; }
            }
        }
    }
    return false;
}

void temporal_show(Board board, CurrentPiece current_piece, StatesVariables s){ // Modified with AI.
    printf("\n     ");
    for(int j = 0; j < BOARD_WIDTH + 2; j++){ printf("#"); }
    printf("\n");

    for(int i = 0; i < BOARD_HEIGHT; i++){
        if(i == 2){ printf("HOLD "); }
        else if(i >= 3 && i <= 6){ 
            printf(" %d%d%d%d", s.hold_block.block[i-3][0], s.hold_block.block[i-3][1], 
                s.hold_block.block[i-3][2], s.hold_block.block[i-3][3]); 
        }
        else if(i == 9){ printf("NEXT "); }
        else if(i >= 10 && i <= 13){ 
            printf(" %d%d%d%d", s.next_block.block[i-10][0], s.next_block.block[i-10][1], 
                s.next_block.block[i-10][2], s.next_block.block[i-10][3]); 
        }
        else{ printf("     "); }

        printf("#"); 

        for(int j = 0; j < BOARD_WIDTH; j++){
            int value = board.grid[i][j];

            if(piece_occupies(current_piece, j, i)){ value = current_piece.piece_type; }

            if(value == 0){ printf("."); }
            else{ printf("%d", value); }
        }
        printf("#\n");
    }
    printf("     ");
    for(int j = 0; j < BOARD_WIDTH + 2; j++){ printf("#"); }
    printf("\n");
}

void gotoxy(int x, int y) { // Made with AI.
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x; dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

int main(){
    Board board;
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ board.grid[a][b] = 0; } } // Initialize the board with 0.
    CurrentPiece current_piece = {&board, 0, 0, EMPTY, 0};
    Blocks h_b, nx_p; // Block for hold_block and for next_block.
    for(int i = 0; i < 4; i++){ for(int j = 0; j < 4; j++){ h_b.block[i][j] = 0; nx_p.block[i][j] = 0; } } // Init h_b, nx_p
    StatesVariables states = {&board, 0, 0, 0, 0, 0.8, 0.05, false, EMPTY, true, h_b, EMPTY, nx_p};

    current_piece.piece_type = next_piece();
    states.next_piece_type = next_piece();
    update_difficulty(&states);
    init_keyboard(&current_piece, &states);

    while(states.game_over == false){
        gotoxy(0, 0);
        update(&board, &current_piece, &states);

        cout << endl << "Eliminated lines: " << states.eliminated_lines << "\t";
        cout << "Score: " << states.score << "\t";
        cout << "Level: " << states.level << "\t";
        cout << endl << "Speed: " << states.gravity_time;
        temporal_show(board, current_piece, states);

        if(states.game_over == true){ printf("Game over.\n"); }
        Sleep(33);
    }

    return 0;
}