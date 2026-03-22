# include "current_piece.hpp"
# include "bag_random.hpp"
# include "pieces.hpp"
# include "board.hpp"

void init_current_piece(Board *board, Current_piece *actual){
    actual->game_board = board;
    actual->piece_type = next_piece();
    actual->currect_x = BOARD_WIDTH / 2 - 2;
    actual->currect_y = 0;
    actual->rotation = 0;
}

bool can_move(Current_piece *actual, int new_x, int new_y, int new_rotation){
    Blocks piece_shape;
    get_blocks(actual->piece_type, new_rotation, &piece_shape);

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(piece_shape.block[i][j] == 1){
                int board_x = new_x+j;
                int board_y = new_y+i;
                
                if(board_x<0 || board_x>=BOARD_WIDTH)
                    return false;
                if(board_y>=BOARD_HEIGHT)
                    return false;
                if(board_y>=0 && actual->game_board->grid[board_y][board_x]!=0)
                    return false;
            }
        }
    }
    return true;
}

void move_to_left(Current_piece *actual){
    if(can_move(actual, actual->currect_x-1, actual->currect_y, actual->rotation))
        actual->currect_x -= 1;  
}

void move_to_right(Current_piece *actual){
    if(can_move(actual, actual->currect_x+1, actual->currect_y, actual->rotation))
        actual->currect_x += 1;
}

void rotate(Current_piece *actual){
    int new_rotation=(actual->rotation+1)%4;
    if(can_move(actual, actual->currect_x, actual->currect_y, new_rotation))
        actual->rotation=new_rotation;
}

void go_down(Current_piece *actual){
    if(can_move(actual, actual->currect_x, actual->currect_y+1, actual->rotation))
        actual->currect_y += 1;
    else{
        set_piece(actual->game_board, actual->currect_x, actual->currect_y, actual->piece_type, actual->rotation);
        actual->piece_type=next_piece();
        actual->currect_x=BOARD_WIDTH/2-2;
        actual->currect_y=0;
        actual->rotation=0;

        //bool game_over();
    }
}

/* --- TEST ---
# include  <iostream>
using namespace std;

int main(){
    Board board;
    Current_piece actual;
    for (int i=0; i<BOARD_HEIGHT; i++)
    for (int j=0; j<BOARD_WIDTH; j++)
            board.grid[i][j]=0;

            init_current_piece(&board, &actual);
            
            cout << "Posicion inicial: (" << actual.currect_x << "," << actual.currect_y << ")\n";
    move_to_right(&actual);
    cout << "Despues de derecha: (" << actual.currect_x << "," << actual.currect_y << ")\n";
    move_to_left(&actual); move_to_left(&actual);
    cout << "Despues de izquierda (desde posicion inicial): (" << actual.currect_x << "," << actual.currect_y << ")\n";
    rotate(&actual);
    cout << "Despues de rotar: rotacion=" << actual.rotation << endl;
    for(int i=0; i<5; i++){
        go_down(&actual); cout << "Bajando " << i+1 << ":(" << actual.currect_x << "," << actual.currect_y << ")\n";
    }
    cout << "Nueva pieza generada: " << actual.piece_type << endl;
    return 0;
}
*/