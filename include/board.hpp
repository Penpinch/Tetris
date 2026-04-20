# ifndef BOARD
# define BOARD

typedef enum{
    BOARD_WIDTH = 10, 
    BOARD_HEIGHT = 22
} Dimensions;

typedef struct{
    int grid[BOARD_HEIGHT][BOARD_WIDTH];
} Board;

void set_block(Board *board, int x, int y, int piece_type);
void set_piece(Board *board, int piece_x, int piece_y, int piece_type, int rotation);

# endif 