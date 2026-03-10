# ifndef BOARD
# define BOARD

typedef enum{
    BOARD_WIDTH = 10, 
    BOARD_HEIGHT = 22
} Dimensions;

typedef struct{
    int grid[BOARD_HEIGHT][BOARD_WIDTH];
} Board;

bool is_empty(Board *board, int x, int y);

# endif 