# include "current_piece.hpp"
# include "bag_random.hpp"
# include "pieces.hpp"
# include "board.hpp"

static Board *game_board = nullptr;
static int current_x;
static int current_y;
static int piece_type;
static int rotation;

void init_current_piece(Board *board){
    game_board=board;
    piece_type=next_piece();
    current_x=BOARD_WIDTH/2-2;
    current_y=0;
    rotation=0;
}

bool can_move(int new_x, int new_y, int new_rotation){
    Blocks piece_shape;
    get_blocks(piece_type, new_rotation, &piece_shape);

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(piece_shape.block[i][j] == 1){
                int board_x = new_x+j;
                int board_y = new_y+i;
                
                if(board_x<0 || board_x>=BOARD_WIDTH)
                    return false;
                if(board_y>=BOARD_HEIGHT)
                    return false;
                if(board_y>=0 && game_board->grid[board_y][board_x]!=0)
                    return false;
            }
        }
    }
    return true;
}

void move_to_left(){
    if(can_move(current_x-1, current_y, rotation))
        current_x -= 1;  
}

void move_to_right(){
    if(can_move(current_x+1, current_y, rotation))
        current_x += 1;
}

void rotate(){
    int new_rotation=(rotation+1)%4;
    if(can_move(current_x, current_y, new_rotation))
        rotation=new_rotation;
}

void go_down(){
    if(can_move(current_x, current_y+1, rotation))
        current_y += 1;
    else{
        set_piece(game_board, current_x, current_y, piece_type, rotation);
        piece_type=next_piece();
        current_x=BOARD_WIDTH/2-2;
        current_y=0;
        rotation=0;

        //bool game_over();
    }
}

/* --- TEST ---
# include  <iostream>
using namespace std;

int main(){
    Board board;
    for (int i=0; i<BOARD_HEIGHT; i++)
        for (int j=0; j<BOARD_WIDTH; j++)
            board.grid[i][j]=0;

    init_current_piece(&board);

    cout << "Posicion inicial: (" << current_x << "," << current_y << ")\n";
    move_to_right();
    cout << "Despues de derecha: (" << current_x << "," << current_y << ")\n";
    move_to_left(); move_to_left();
    cout << "Despues de izquierda (desde posicion inicial): (" << current_x << "," << current_y << ")\n";
    rotate();
    cout << "Despues de rotar: rotacion=" << rotation << endl;
    for(int i=0; i<5; i++){
        go_down(); cout << "Bajando " << i+1 << ":(" << current_x << "," << current_y << ")\n";
    }
    cout << "Nueva pieza generada: " << piece_type << endl;
    return 0;
}
*/