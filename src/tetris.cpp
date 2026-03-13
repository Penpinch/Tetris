# include <iostream>
# include "pieces.hpp"
using namespace std;

// -- main loop --

int main(){
    Blocks b;
    int pce_inx = 6, rot_inx = 0;
    
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


    return 0;
}