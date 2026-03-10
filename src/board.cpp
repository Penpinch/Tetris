# include "board.hpp"
// Manage the playflield.
//is_empty(x, y), set_block(), is_full(row), set_piece()
bool is_empty(Board *board, int x, int y){
    return (board->grid[x][y] == 0) ? true : false;
}