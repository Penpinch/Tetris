# include "raylib.h"
# include <math.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <time.h>

# include "current_piece.hpp"
# include "core.hpp"
# include "bag_random.hpp"
# include "board.hpp"
# include "lines.hpp"
# include "pieces.hpp"

KeysFunctions key_actions[256] = {NULL}; // Functon pointer.

void write_file(char best_user[][4], long int best_scores[]){
    FILE *scores_file = fopen("../scores_file.txt", "w");
    if(scores_file == NULL){ perror("Failed at opening the file."); return; }

    fprintf(scores_file, "%s %s %s %s %s %ld %ld %ld %ld %ld", 
        best_user[0], best_user[1], best_user[2], best_user[3], best_user[4],
        best_scores[0], best_scores[1], best_scores[2], best_scores[3], best_scores[4]);
    fclose(scores_file);
}

void read_file(char best_user[][4], long int read_scores[]){
    FILE *scores_file = fopen("../scores_file.txt", "r");
    if(scores_file == NULL){ perror("Failed at opening the file."); return; }

    if(fscanf(scores_file, "%s %s %s %s %s %ld %ld %ld %ld %ld", 
        best_user[0], best_user[1], best_user[2], best_user[3], best_user[4],
        &read_scores[0], &read_scores[1], &read_scores[2], &read_scores[3], &read_scores[4]) != 10){
        perror("Error while reading scores. ");
    }
    fclose(scores_file);
}

bool capture_name(char best_user[][4], int position){
    if(position == -1){ return false; }

    static char temporal_buffer[4] = ""; 
    static int cursor = 0;

    int key = GetCharPressed();

    while(key > 0){

        if(((key >= 65 && key <= 90) || (key >= 97 && key <= 122) || (key >= 48 && key <= 50)) && cursor < 3){
                temporal_buffer[cursor++] = (char)key;
                temporal_buffer[cursor] = '\0';
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE) && cursor > 0){ cursor--; temporal_buffer[cursor] = '\0'; }
        
        strcpy(best_user[position], temporal_buffer);
        
        if(IsKeyPressed(KEY_ENTER) && cursor > 0){ 
            temporal_buffer[0] = '\0';
            cursor = 0;
            return false;
        }
    return true;
}

int update_best_scores(StatesVariables *states){
    for(int i = 0; i < 5; i++){
        if(states->score > states->best_scores[i]){
            for(int j = 4; j > i; j--){ 
                states->best_scores[j] = states->best_scores[j - 1]; 
                strcpy(states->best_user[j], states->best_user[j - 1]);
            }
            states->best_scores[i] = states->score;
            strcpy(states->best_user[i], "-");
            return i;
        }
        if(states->score == states->best_scores[i]){ return -1; }
    }
    return -1;
}

void update_score(StatesVariables *states, int eliminated_lines){
    switch(eliminated_lines){
        case 0: eliminated_lines = 0; break;
        case 1: eliminated_lines = SINGLE_; break;
        case 2: eliminated_lines = DOUBLE_; break;
        case 3: eliminated_lines = TRIPLE_; break;
        case 4: eliminated_lines = TETRIS; break;
    }
    states->score += eliminated_lines * (states->level + 1);
}

void update_difficulty(StatesVariables *states){
    states->level = states->choosed_level + (states->eliminated_lines / 10);
    float delay = 0.8f * pow(0.9f, states->level - 1);

    if(delay < 0.1f){ states->gravity_time = 0.1f; }
    else { states->gravity_time = delay; }
}

void spawn_next_piece(StatesVariables *states, CurrentPiece *current_piece){
    current_piece->piece_type = states->next_piece_type;
    states->next_piece_type = next_piece();
    memcpy(states->next_block.block, tetris_pieces[states->next_piece_type - 1][0], sizeof(int) * 16);

    current_piece->current_x = (BOARD_WIDTH >> 1) - 2; // Same than '/2'. Just to use Bitwise operators.
    current_piece->current_y = 0;
    current_piece->rotation = 0;
    states->can_be_held = true;

    if(can_move(current_piece, current_piece->current_x, current_piece->current_y, current_piece->rotation) == false){
        states->game_over = true;
    }
}

void gravity(StatesVariables *states, CurrentPiece *current_piece, bool soft_drop){
    static float start = 0;
    float end = GetTime();    
    float time_difference = end - start;

    float speed = (soft_drop) ? states->fast_gravity_time : states->gravity_time;

    if(time_difference >= (float)speed){
        int go_down_result = go_down(current_piece); 
        if(go_down_result <= 0){ spawn_next_piece(states, current_piece); }
        start = end; 
    }
}

void hold_piece(CurrentPiece *current_piece, StatesVariables *states){
    if(states->can_be_held == false){ return; }
    if(states->hold_piece_type == EMPTY){
        states->hold_piece_type = current_piece->piece_type;
        memcpy(states->hold_block.block, tetris_pieces[states->hold_piece_type - 1][0], sizeof(int)*16);
        current_piece->piece_type = next_piece();
    } else {
        int temp = current_piece->piece_type;
        current_piece->piece_type = states->hold_piece_type;
        states->hold_piece_type = temp;
        memcpy(states->hold_block.block, tetris_pieces[temp - 1][0], sizeof(int)*16);
    }
    current_piece->current_x = (BOARD_WIDTH >> 1) - 2;
    current_piece->current_y = 0;
    current_piece->rotation = 0;
    states->can_be_held = false;
}

void hard_drop(CurrentPiece *current_piece, StatesVariables *states){
    while((go_down(current_piece)) == 1){}
    spawn_next_piece(states, current_piece);
}

void mute_unmute_music(Music music){
    if(IsMusicStreamPlaying(music)){ PauseMusicStream(music); } 
    else { ResumeMusicStream(music); }
}

void pause(CurrentPiece *current_piece, StatesVariables *states){ 
    if(states->paused == false){ states->paused = true; }
    else{ states->paused = false; }
}

void reset(CurrentPiece *current_piece, StatesVariables *states){
    for(int a = 0; a < BOARD_HEIGHT; a++){ for(int b = 0; b < BOARD_WIDTH; b++){ states->board->grid[a][b] = 0; } } // Reset board.

    current_piece->piece_type = next_piece();
    current_piece->current_x = 3;
    current_piece->current_y = 0;
    current_piece->rotation = 0;

    states->score = 0;
    states->choosed_level = 0;
    states->level = 0;
    states->eliminated_lines = 0;
    states->gravity_time = 0.8;
    states->fast_gravity_time = 0.05;
    states->game_over = false;
    states->paused = false;
    states->hold_piece_type = EMPTY; 
    states->can_be_held = true; 
    for(int i = 0; i < 4; i++){ for(int j = 0; j < 4; j++){ states->hold_block.block[i][j] = 0; } } // Reset hold_block.
    states->next_piece_type = next_piece();
    get_blocks(states->next_piece_type, 0, &states->next_block);
    states->new_game = false;
}   

void init_keyboard(CurrentPiece *current_piece, StatesVariables *states){ // Just to practice function pointers.
    key_actions[KEY_UP] = rotate_right; // ROTATION TO THE RIGHT
    key_actions[KEY_SPACE] = hard_drop; // HARD DROP
    key_actions[KEY_C] = hold_piece; //HOLD
    key_actions[KEY_Z] = rotate_left; // ROTATION TO THE LEFT
}

void input(CurrentPiece *current_piece, StatesVariables *states, InputState *input_state){ // Made with a lot of AI guidance.
    static float move_timer = 0;
    float move_delay = 0.09f; // Lateral movements every 0.09 seconds.
    if(IsKeyPressed(KEY_P)){ states->exit_raylib_window = true; }

    if(states->paused == true || states->game_over == true){ return; }

    int keys[] = {KEY_C, KEY_Z, KEY_UP, KEY_SPACE};
    int active_keys = sizeof(keys) / sizeof(keys[0]);

    for(int i = 0; i < active_keys; i++){
        int key = keys[i];

        if(IsKeyPressed(key) == true){
            if(key_actions[key] != NULL){ key_actions[key](current_piece, states); }
        }
    }

    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)){
        move_timer += GetFrameTime();
        if(move_timer >= move_delay){
            if(IsKeyDown(KEY_LEFT)){ move_to_left(current_piece, states); }
            if(IsKeyDown(KEY_RIGHT)){ move_to_right(current_piece, states); }
            move_timer = 0;
        }
    } else{ move_timer = move_delay; }

    input_state->soft_drop = IsKeyDown(KEY_DOWN);
}

void update_level(StatesVariables *states){
    if(states->choosed_level < 20){ states->choosed_level += 5; }
    else { states->choosed_level = 0; }
    update_difficulty(states);
}

void update(Board *board, CurrentPiece *current_piece, StatesVariables *states){
    InputState input_state; // SOFT DROP
    input(current_piece, states, &input_state);

    if(states->paused == true || states->game_over == true){ return; }

    gravity(states, current_piece, input_state.soft_drop);

    int eliminated_lines_single_move = check_lines(board);
    update_score(states, eliminated_lines_single_move);
    states->eliminated_lines += eliminated_lines_single_move;

    update_difficulty(states);
}