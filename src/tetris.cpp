# include <iostream>
# include "pieces.hpp"
using namespace std;

// -- main loop --

int main(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d", tetris_pieces[0][3][i][j]);
        }
        printf("\n");
    }

    return 0;
}