# include <iostream>
using namespace std;
# include "board.hpp"
# include "pieces.hpp"
# include "lines.hpp"

// -- main loop --

int main(){
    Board board;
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ board.grid[a][b] = 0; } }
    Blocks b;
    int elmtd_lines = 0;

    set_piece(&board, 19, -1, PIECE_L, 3);
    set_piece(&board, 19, 7, PIECE_J, 0);
    set_piece(&board, 18, 4, PIECE_O, 0);
    set_piece(&board, 19, 5, PIECE_S, 1);
    set_piece(&board, 19, 2, PIECE_Z, 1);
    set_piece(&board, 20, 0, PIECE_I, 1);
    set_piece(&board, 17, 6, PIECE_T, 1);

    elmtd_lines = check_lines(&board);

    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board.grid[i][j] == 0){ cout << "*"; } 
            else { cout << board.grid[i][j]; }
        }
        cout << endl;
    }

/*
while(rot_inx < 4){
    get_blocks(pce_inx, rot_inx, &b);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d", b.block[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    rot_inx++;
}
*/    


    return 0;
}