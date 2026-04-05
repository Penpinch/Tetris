# include <iostream>
using namespace std;
# include <windows.h>

# include "bag_random.hpp"
# include "board.hpp"
# include "core.hpp"
# include "current_piece.hpp"
# include "lines.hpp"
# include "pieces.hpp"

// -- main loop --

void temporal_show(Board board){
    cout << endl;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board.grid[i][j] == 0){ cout << "*"; } 
            else { cout << board.grid[i][j]; }
        }
        cout << endl;
    }
    cout << endl;
}

int main(){
    Board board;
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ board.grid[a][b] = 0; } } // Initialize the board with 0.
    CurrentPiece current_piece = {&board, 0, 0, EMPTY, 0};
    StatesVariables states = {&board, 0, 0, 0, 0.5};

    current_piece = {&board, 7, 10, PIECE_J, 0};
    set_piece(&board, 0, 20, PIECE_I, 1);
    set_piece(&board, 4, 20, PIECE_I, 1);

    int game_on = true, cont = 0;
    while(game_on){
        cout << cont;
        update(&board, &current_piece, &states);
        
        states.eliminated_lines += check_lines(&board);
        cout << endl << "Eliminated lines: " << states.eliminated_lines << endl;
        temporal_show(board);

        if(states.eliminated_lines == 1){ game_on = false; }
        if(cont >= 19){ game_on = false; }
        Sleep(500);
        cont ++;
    }

    return 0;
}