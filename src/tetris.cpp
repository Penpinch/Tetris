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

void temporal_show(Board board, CurrentPiece current_piece, StatesVariables s){ // Esto se ve terrible. Que horror.
    cout << endl;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        if(1 < i && i <= 7){
            switch(i){
                case 2:
                    printf("|----|"); break;
                case 3:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.hold_block.block[0][i]); }; printf("|"); break;
                case 4:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.hold_block.block[1][i]); }; printf("|"); break;
                case 5:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.hold_block.block[2][i]); }; printf("|"); break;
                case 6:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.hold_block.block[3][i]); }; printf("|"); break;
                case 7:
                    printf("|----|"); break;
            }
        } else if(8 < i && i <= 14){
            switch(i){
                case 9:
                    printf("|----|"); break;
                case 10:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.next_block.block[0][i]); }; printf("|"); break;
                case 11:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.next_block.block[1][i]); }; printf("|"); break;
                case 12:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.next_block.block[2][i]); }; printf("|"); break;
                case 13:
                    printf("|"); for(int i = 0; i < 4; i++){ printf("%d", s.next_block.block[3][i]); }; printf("|"); break;
                case 14:
                    printf("|----|"); break;
            }
        } else { printf("      "); }

        for(int j = 0; j < BOARD_WIDTH; j++){
            int value = board.grid[i][j];
            if(piece_occupies(current_piece, j, i)){ value = current_piece.piece_type; }
            (value == 0) ? printf("*") : printf("%d", value);
        }
    cout << endl;
    }
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
    int game_on = true;
    while(game_on){
        update(&board, &current_piece, &states);

        if(states.game_over == true){ printf("Game over."); game_on = false; }

        cout << endl << "Eliminated lines: " << states.eliminated_lines << "\t";
        cout << "Score: " << states.score << "\t";
        cout << "Level: " << states.level << "\t";
        cout << endl << "Speed: " << states.gravity_time;
        temporal_show(board, current_piece, states);

        Sleep(33);
        if(game_on == true){ system("cls"); }
    }

    return 0;
}