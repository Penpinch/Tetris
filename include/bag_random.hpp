# ifndef BAG_RANDOM
# define BAG_RANDOM

#define EXISTING_PIECES 7

typedef struct{
    int elements[EXISTING_PIECES];
    int front, back, existing_elements;
} Queue;

extern Queue bag;

void swap(int *num1, int *num2);
void shuffle(int arr[]);
void queue_init(Queue *bag);
bool empty(Queue *bag);
bool full(Queue *bag);
void queue_back_push(Queue *bag, int value);
void queue_front_pop(Queue *bag);
int front(Queue *bag);
int next_piece();

# endif