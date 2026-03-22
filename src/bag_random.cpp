# include "bag_random.hpp"
# include <stdlib.h>
# include <time.h>

// #include <stdio.h>

void swap(int *num1, int *num2){ // Made with Google help.
    int temporal = *num1;
    *num1 = *num2;
    *num2 = temporal;
}

void shuffle(int arr[]){  // Made with Google help.
    int j = 0;

    for(int i = EXISTING_PIECES - 1; i > 0; i--){
        j = rand() % EXISTING_PIECES;
        swap(&arr[i], &arr[j]);
    }
}

bool empty(Queue *bag){ return (bag->existing_elements == 0) ? true : false; }

bool full(Queue *bag){ return (bag->existing_elements == EXISTING_PIECES) ? true : false; }

void queue_back_push(Queue *bag, int value){
    bag->existing_elements++;
    bag->elements[(bag->back)++] = value;
    
    if(bag->back == EXISTING_PIECES){ bag->back = 0; }
}

void queue_front_pop(Queue *bag){
    if(empty(bag)){ 
        int new_bag[EXISTING_PIECES] = {0, 1, 2, 3, 4, 5, 6};
        shuffle(new_bag); 

        for(int i = 0; i < EXISTING_PIECES; i++){ queue_back_push(bag, new_bag[i]); }
        // return;
    }
    bag->existing_elements --;
    bag->front++;
    if(bag->front == EXISTING_PIECES){ bag->front = 0; }
}

int front(Queue *bag){
    if(!empty(bag)){ return bag->elements[bag->front]; }
    return -1;
}

void queue_init(Queue *bag){
    srand(time(NULL));
    bag->front = 0;
    bag->back = 0;
    bag->existing_elements = 0;
}

int next_piece(){
    static Queue bag;
    static bool initialize = false;
    int next_on_queue = 0;
    if(initialize == false){
        queue_init(&bag);
        initialize = true;
    }
    queue_front_pop(&bag);
    next_on_queue = front(&bag);

    return next_on_queue;
}

/* --- TEST ---
int main(){
    for(int i = 0; i < EXISTING_PIECES; i++){ int test = next_piece(); printf("Next piece: %d\n", test); }
    return 0;
}
*/