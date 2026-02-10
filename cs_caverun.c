// cs_caverun.c
// Written by Anthena Su z5640267 on 13/03/2025
//
// Description: player aims to collect gems & dirt reaching unlocked exit 

// Provided Libraries
#include <stdio.h>

// Add your own #include statements below this line
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

// Provided constants
#define COLS 10
#define ROWS 10
#define INVALID_ROW -1
#define INVALID_COL -1
#define INITIAL_LIVES 3

// Add your own #defines constants below this line
#define MAX_COMMANDS 100
#define TRUE 1
#define FALSE 0

// Provided Enums
// Enum for features on the game board
enum entity {
    EMPTY,
    DIRT,
    WALL,
    BOULDER,
    GEM,
    EXIT_LOCKED,
    EXIT_UNLOCKED,
    HIDDEN,
    LAVA,
    PLAYER
};

// Add your own enums below this line

// Represents a tile/cell on the game board
struct tile {
    enum entity entity;
};

// Add your own structs below this line

// Provided Function Prototypes
void initialise_board(struct tile board[ROWS][COLS]);
void print_board(
    struct tile board[ROWS][COLS], 
    int player_row, 
    int player_col,
    int lives_remaining);
void print_board_line(void);
void print_board_header(int lives);
void prt_map_stcs(struct tile board[ROWS][COLS], int current, int max, int mode);

// Add your function prototypes below this line
bool check_validity (struct tile board[ROWS][COLS], int input_row, int input_col);
bool comw_validity (
    struct tile board[ROWS][COLS], 
    int start_row, 
    int end_row, 
    int start_col, 
    int end_col, 
    int initial_row, 
    int initial_col);
int movement(struct tile board[ROWS][COLS], char direction, int player_row, int player_col, int mode); 
bool check_movement(struct tile board[ROWS][COLS], int r, int c);
int player_row (struct tile board[ROWS][COLS]);
int player_col (struct tile board[ROWS][COLS]);
int check_collection(struct tile board[ROWS][COLS], int r, int c, int mode);
int get_max_score (struct tile board[ROWS][COLS], int mode);
int num_clct(struct tile board[ROWS][COLS]);
bool check_win(struct tile board[ROWS][COLS]);
int prchk_mv(struct tile board[ROWS][COLS], char direction, int player_row, int player_col);
void boulder_falling(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *lives, 
    int *irow, 
    int *icol, 
    bool *play, 
    char gravity_direction, 
    int *counter_lv,  
    int score);
void illuminate(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS], int radius, int lives);
void first_copy_board(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS]);
void copy_board(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS]);
void game_setup(struct tile board[ROWS][COLS], int *initial_row, int *initial_col);
void commandl (struct tile temp[ROWS][COLS], int input_row, int input_col);
void commands (struct tile board[ROWS][COLS]);
void commandw(
    struct tile board[ROWS][COLS], 
    int start_row, 
    int start_col, 
    int end_row, 
    int end_col, 
    int *initial_row, 
    int *initial_col);
void check_wbge(
    struct tile board[ROWS][COLS], 
    char player_command,
    int command_row, 
    int command_col, 
    int *initial_row, 
    int *initial_co);
void map_feature (
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *initial_row, 
    int *initial_col);
void i_direction(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS], int radius, int lives);
void dash_movement(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *breath, 
    char d_one, 
    char d_two, 
    int lives, 
    int *score,
    int *win, 
    int mode);
void check_lives(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int radisu, 
    int score, 
    int lives, 
    bool play, 
    int counter_lv);
void printf_gravity(char gravity_direction);
void hit(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int row, 
    int col, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int shift_x, 
    int shift_y, 
    int *counter_lv, 
    int score); 
void encounter_lava(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int player_row,
    int player_col,
    char direction, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int *score,
    int *counter_lv);
void str_cmd_sqnce(char pst_cmd[], char direction, int *len);
void chk_cmd_sqnce(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS], char pst_cmd[], char direction, int *len, int *mode);
void game_lava_mode(struct tile temp[ROWS][COLS], int *mode, int counter_lv); 
void lava_seeds_mode(struct tile temp[ROWS][COLS], int *mode, int counter_lv);
int count_neighbor (struct tile temp[ROWS][COLS], int target_row, int target_col);
void change_lava(struct tile temp_lava[ROWS][COLS], struct tile temp[ROWS][COLS]);
void encounter_lava_respawn (
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int ply_row,
    int ply_col,
    char direction, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int *score,
    int *counter_lv);

// Provided sample main() function (you will need to modify this)
int main(void) {
    struct tile board[ROWS][COLS];
    struct tile temp[ROWS][COLS];
    char pst_cmd[MAX_COMMANDS] = {0};  
    int len = 0;
    int initial_row, initial_col = 0;
    int mode = 0;

    // Game setup & Declare map feature
    initialise_board(board);
    game_setup(board, &initial_row, &initial_col);
    printf("Enter map features: \n");
    first_copy_board(board, temp);
    map_feature (board, temp, &initial_row, &initial_col);
    copy_board(board, temp);
    print_board(temp, initial_row, initial_col, 3);
    int max_clct =  num_clct(board);

    // Start playing game
    int score = 0; 
    char direction[3];
    int breath = 0;
    bool play = true; 
    int radius = 0; 
    int lives = 3;
    char gravity_direction = 's'; 
    int win = 0;
    int counter_lv = 0; 

    printf("--- Gameplay Phase ---\n");
    void game_phase(struct tile board[ROWS][COLS]);
    // read the input command 
    while (scanf(" %2s", direction) == 1) {
        if (direction[0] == 'q') {
            printf("--- Quitting Game ---\n");
            break;
        }
        copy_board(board, temp);
        if (direction[0] == 'i') {
            scanf(" %d", &radius);
            i_direction(board, temp, radius, lives);
        }
        else if (direction[0] == 'p') {
            printf("You have %d point(s)!\n", score);
        } 
        else if (direction[0] == 'm') {
            prt_map_stcs(board, max_clct - num_clct(board), max_clct, mode);
        }
        else if (direction[0] == 'L') {
            chk_cmd_sqnce(board, temp, pst_cmd, direction[0], &len, &mode);
        }
        else {
            if (direction[0] == 'g') {
                scanf(" %c", &gravity_direction);
                printf_gravity(gravity_direction);
            }
            char dash_one = direction[0];
            char dash_two = direction[1];
            // Dash Process movement 
            if (direction[1] != '\0') {
                if (counter_lv == 0) {
                    game_lava_mode(temp, &mode, counter_lv);
                    lava_seeds_mode(temp, &mode, counter_lv);
                }
                str_cmd_sqnce(pst_cmd, dash_one, &len);
                str_cmd_sqnce(pst_cmd, dash_two, &len);
                dash_movement(board, temp, &breath, dash_one, dash_two, lives, &score, &win, mode);
                if (win == 1) {
                    break;
                }
                encounter_lava(board, temp, player_row(board), player_col(board), direction[0], &lives, &play, initial_row, initial_col, &score, &counter_lv);
            } else {
                //check if encounter lava or not
                encounter_lava(board, temp, player_row(board), player_col(board), direction[0], &lives, &play, initial_row, initial_col, &score, &counter_lv);
                str_cmd_sqnce(pst_cmd, direction[0], &len);
                if (counter_lv == 0) {
                    score += movement(board, direction[0], player_row(board), player_col(board), mode);
                    // Check win
                    if (check_win(board) && score >= 1000) {
                        copy_board(board, temp);
                        print_board(temp, player_row(board), player_col(board), lives);
                        printf("You Win! Final Score: %d point(s)!\n", score - 1000);
                        break;
                    } 
                } else {
                    counter_lv = 0;  
                }
                copy_board(board, temp);
                breath = 0;
            }
            //after player moves boulder moves
            boulder_falling(board, temp, &lives, &initial_row, &initial_col, 
                            &play, gravity_direction, &counter_lv, score);
            copy_board(board, temp);
            //change the map according to game_lava_mode or lava_seeds_mode 
            game_lava_mode(temp, &mode, counter_lv);
            lava_seeds_mode(temp, &mode, counter_lv);
            copy_board(board, temp);
            check_lives(board, temp, radius, score, lives, play, counter_lv);
            if (play == false || lives == 0) {
                break;
            }
            illuminate(board, temp, radius, lives);
            print_board(temp, player_row(board), player_col(board), lives);
        }
    }
    return 0;
    print_board(board, INVALID_ROW, INVALID_COL, INITIAL_LIVES);
}

// Add your function definitions below this line

// Store the command in a list 
void str_cmd_sqnce(char pst_cmd[], char direction, int *len) {
    // Only store if we have space
    if (*len < MAX_COMMANDS) {
        pst_cmd[*len] = direction;
        (*len)++;
    }
}

// Determine whether it is game lava mode or lava seeds mode
void chk_cmd_sqnce(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    char pst_cmd[], 
    char direction, 
    int *len,
    int *mode) {
    pst_cmd[*len] = direction;
    (*len)++;

    if (*len >= 5) {
        // Check for "wasdL" sequence 
        if ((pst_cmd[*len-5] == 'w' && 
            pst_cmd[*len-4] == 'd' &&
            pst_cmd[*len-3] == 's' &&
            pst_cmd[*len-2] == 'a' &&
            pst_cmd[*len-1] == 'L')) {
            *mode = 1; 
            printf("Game Of Lava: Activated\n");
            return; 

            return; 
        }
        // Check for "dsawL" sequence 
        else if ((pst_cmd[*len-5] == 'w' && 
            pst_cmd[*len-4] == 'a' &&
            pst_cmd[*len-3] == 's' &&
            pst_cmd[*len-2] == 'd' &&
            pst_cmd[*len-1] == 'L')) {
            printf("Lava Seeds: Activated\n");
            *mode = 2; 
            return; 
        }
    }
}

void lava_seeds_mode(struct tile temp[ROWS][COLS], int *mode, int counter_lv) {
    if (*mode != 2 || counter_lv == 2) {
        return;
    }
    struct tile temp_lava_seeds[ROWS][COLS]; 
    
    // Copy an empty one to temp_lava_seed 
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            temp_lava_seeds[row][col].entity = EMPTY;
        }    
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int count = count_neighbor(temp, i, j);
            
            //Birth
            if (temp[i][j].entity != LAVA && count == 2) {
                temp_lava_seeds[i][j].entity = LAVA; 
            }
            //Survivial & Death
            else {
                temp_lava_seeds[i][j].entity = EMPTY; 
            }

        }
    }

    change_lava(temp_lava_seeds, temp); 
}

void game_lava_mode(struct tile temp[ROWS][COLS], int *mode, int counter_lv) {
    if (*mode != 1) {
        return;
    }
    // Create another temp board to store lava 
    struct tile temp_lava[ROWS][COLS]; 
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            temp_lava[row][col].entity = EMPTY;
        }    
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int count = count_neighbor(temp, i, j);
            if (temp[i][j].entity != LAVA && count == 3) {
                temp_lava[i][j].entity = LAVA; 
            }
            else if (temp[i][j].entity == LAVA && (count == 3 || count == 2)) {
                temp_lava[i][j].entity = LAVA; 
            }
        }
    }

    change_lava(temp_lava, temp); 
}

// Count how many neighbors there are
int count_neighbor(
    struct tile temp[ROWS][COLS], 
    int target_row, 
    int target_col) {

    int count = 0;
    
    for (int row_offset = -1; row_offset <= 1; row_offset++) {
        for (int col_offset = -1; col_offset <= 1; col_offset++) {
            // Skip the center tile 
            if (row_offset == 0 && col_offset == 0) {
                continue;
            }
            
            // Calculate wrapped coordinates
            int neighbor_row = (target_row + row_offset + ROWS) % ROWS;
            int neighbor_col = (target_col + col_offset + COLS) % COLS;
            
            if (temp[neighbor_row][neighbor_col].entity == LAVA) {
                count++;
            }
        }
    }
    return count;
}

// Copy the temp_lava to temp 
void change_lava(
    struct tile temp_lava[ROWS][COLS], 
    struct tile temp[ROWS][COLS]) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            temp[i][j] = temp_lava[i][j];
        }
    }
}


// Check if encounter lava or not 
void encounter_lava(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int ply_row,
    int ply_col,
    char direction, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int *score,
    int *counter_lv) {
    if (direction == 'w' || direction == 'W') {
        if (ply_row != 0) {
            if (temp[ply_row - 1][ply_col].entity == LAVA) {
                encounter_lava_respawn (board, temp, ply_row, ply_col, direction, lives, play, irow, icol, score, counter_lv);
                if (board[ply_row - 1][ply_col].entity == GEM) {
                    (*score) += 200;
                }
                else if (board[ply_row - 1][ply_col].entity == DIRT) {
                    (*score) += 10;
                }
                if (*counter_lv != 2) {
                    board[ply_row - 1][ply_col].entity = EMPTY; 
                }
            }
        }
    }
    if (direction == 'a' ||direction == 'A') {
        if (ply_col != 0) {
            if (temp[ply_row][ply_col - 1].entity == LAVA) {
                encounter_lava_respawn (board, temp, ply_row, ply_col, direction, lives, play, irow, icol, score, counter_lv);                
                if (board[ply_row][ply_col - 1].entity == GEM) {
                    (*score) += 200;
                }
                else if (board[ply_row][ply_col - 1].entity == DIRT) {
                    (*score) += 10;
                }
                if (*counter_lv != 2) {
                    board[ply_row][ply_col - 1].entity = EMPTY; 
                }
            }
        }
    }
    if (direction == 's' || direction == 'S') {
        if (ply_row != 9) {
            if (temp[ply_row + 1][ply_col].entity == LAVA) {
                encounter_lava_respawn (board, temp, ply_row, ply_col, direction, lives, play, irow, icol, score, counter_lv);           
                if (board[ply_row + 1][ply_col].entity == GEM) {
                    (*score) += 200;
                }
                else if (board[ply_row + 1][ply_col].entity == DIRT) {
                    (*score) += 10;
                }
                if (*counter_lv != 2) {
                    board[ply_row + 1][ply_col].entity = EMPTY; 
                }
            }
        }
    }
    if (direction == 'd' || direction == 'D') {
        if (ply_col != 9) {
            if (temp[ply_row][ply_col + 1].entity == LAVA) {
                encounter_lava_respawn (board, temp, ply_row, ply_col, direction, lives, play, irow, icol, score, counter_lv);
                if (board[ply_row][ply_col + 1].entity == GEM) {
                    (*score) += 200;
                }
                else if (board[ply_row][ply_col + 1].entity == DIRT) {
                    (*score) += 10;
                }
                if (*counter_lv != 2) {
                    board[ply_row][ply_col + 1].entity = EMPTY; 
                }
            }
        }
    }
    if (direction == 'r' ) {
        if (temp[ply_row][ply_col].entity == LAVA) {
            encounter_lava_respawn(board, temp, ply_row, ply_col, direction, lives, play, irow, icol, score, counter_lv);
            (*lives) -= 1;
        }
    }
}

// Check if the player can respawn or toast (end game)
void encounter_lava_respawn (
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int ply_row,
    int ply_col,
    char direction, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int *score,
    int *counter_lv) {

    board[ply_row][ply_col].entity = EMPTY;

    if (check_movement(board, irow, icol) == 1) {
        // Condition I: Can respawn
        if (temp[irow][icol].entity != LAVA && *lives > 1) {
            printf("Respawning!\n");
            board[irow][icol].entity = PLAYER;
            temp[irow][icol].entity = PLAYER; 

            *lives = *lives - 1;
            *counter_lv = 1;

        }
        // Condition II: one last chance
        else if (temp[irow][icol].entity != LAVA && *lives == 1) {
            *lives = *lives - 1;

            *counter_lv = 1;
            if (direction == 'w') {
                board[ply_row - 1][ply_col].entity = PLAYER;
                temp[ply_row - 1][ply_col].entity = PLAYER;
            }
            else if (direction == 'd') {
                board[ply_row][ply_col + 1].entity = PLAYER;
                temp[ply_row][ply_col + 1].entity = PLAYER;

            }
            else if (direction == 's') {
                board[ply_row + 1][ply_col].entity = PLAYER;
                temp[ply_row + 1][ply_col].entity = PLAYER;

            }
            else if (direction == 'a') {
                board[ply_row][ply_col - 1].entity = PLAYER;
                temp[ply_row][ply_col - 1].entity = PLAYER;
            }
            else {
                board[ply_row][ply_col].entity = PLAYER;
                temp[ply_row][ply_col].entity = PLAYER;
            }
            return;
        }
        // Condition III: cant respawn bc lava is in initial pos
        else {
            printf("Respawn blocked! ");
            printf("You're toast! Final score: %d points.\n", *score);
            (*lives) -= 1;
            *play = 0; 
            *counter_lv = 2;
            return; 
        }
  
        if (*counter_lv != 1) {
            copy_board(board, temp);
        }
    }    
}

// Set up the game by taking player inputs 
void game_setup(
    struct tile board[ROWS][COLS], 
    int *initial_row, 
    int *initial_col) {

    printf("Welcome to CS Caverun!\n\n");
    printf("--- Game Setup Phase ---\n");
    printf("Enter the player's starting position: ");
    scanf(" %d %d", initial_row, initial_col);
    while (1) {
        if (*initial_row < ROWS && *initial_row >= 0 
            && *initial_col < COLS && *initial_col >= 0) {
            print_board(board, *initial_row, *initial_col, 3);
            board[*initial_row][*initial_col].entity = PLAYER;
            break;
        }
        else {
            printf("Position %d %d is invalid!\n", *initial_row, *initial_col);
            printf("Enter the player's starting position: ");
            scanf(" %d %d", initial_row, initial_col);
        }
    }

}

// taking player inputs and store in maps according to commands 
void map_feature (
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *initial_row, 
    int *initial_col) {

    char player_command = '\0';
    int start_row, start_col, end_row, end_col;
    int cmd_row = 0;
    int cmd_col = 0;

    while (scanf(" %c", &player_command) == 1) {
        // Leave the setting phase and go to game section
        if (player_command == 's') {
            commands (board); 
            break; 
            
        }
        if (player_command == 'W') {
            scanf(" %d %d %d %d", &start_row, &start_col, &end_row, &end_col);
            commandw(board, 
                    start_row, 
                    start_col, 
                    end_row, 
                    end_col, 
                    initial_row, 
                    initial_col);
        }
        else {
            scanf(" %d %d", &cmd_row, &cmd_col);
            // check w, b, g input validity 
            if (player_command == 'l') {
                
                // check if the input row&col are in map 
                if (check_validity (board, cmd_row, cmd_col)) {
                    // check if occupied 
                    if (board[cmd_row][cmd_col].entity == DIRT 
                    && (cmd_row != *initial_row || cmd_col != *initial_col)) {
                        commandl (temp, cmd_row, cmd_col);
                    }
                    else {
                        printf("Invalid location: tile is occupied!\n");
                        //break;
                    }
                }
                else {
                    printf("Invalid location: position is not on map!\n");
                }
            }
            else {
                check_wbge(board, player_command, 
                cmd_row, cmd_col, initial_row, initial_col);
            }
        }
    }
}

// Make the inputs row col LAVA 
void commandl (struct tile temp[ROWS][COLS], int input_row, int input_col) {  
    temp[input_row][input_col].entity = LAVA;
}

// When player inputs 's' 
void commands (struct tile board[ROWS][COLS]) {
    if (check_win(board) == true) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (board[row][col].entity == EXIT_LOCKED) {
                    board[row][col].entity = EXIT_UNLOCKED;
                }
            }
        }
        return;
    }
}

// When player inputs 'w' 
void commandw(
    struct tile board[ROWS][COLS], 
    int start_row, 
    int start_col, 
    int end_row, 
    int end_col, 
    int *initial_row, 
    int *initial_col) {
    bool check; 
    bool check_if_occupied = true; 
    while (1) {
        check = check_validity(board, start_row, start_col) && 
                check_validity(board, end_row, end_col);
        if (check) {
            check_if_occupied = comw_validity (board, 
                                            start_row, 
                                            end_row, 
                                            start_col, 
                                            end_col, 
                                            *initial_row, 
                                            *initial_col);
        }
        if (check == 0 || check_if_occupied == 0) {
            printf("Invalid location: feature cannot be placed here!\n");
            break;
        }
        else {
            for(int i = start_row; i <= end_row; i++) {
                for(int j = start_col; j <= end_col; j++) {
                    board[i][j].entity = WALL;
                }
            }
            break;
        }
    }
}

// When player inputs 'w', 'g', 'b', 'e' for commands 
void check_wbge(
    struct tile board[ROWS][COLS], 
    char player_command,
    int command_row, 
    int command_col, 
    int *initial_row, 
    int *initial_col)
    {
    bool check;
    while (1) {
        check = check_validity(board, command_row, command_col);
        if (check) {
            if (board[command_row][command_col].entity != DIRT || 
            (command_row == *initial_row && command_col == *initial_col)) {
                printf("Invalid location: tile is occupied!\n");
                break;
            }
            else {
                if (player_command == 'w') {
                    board[command_row][command_col].entity = WALL; 
                }
                if (player_command == 'b') {
                    board[command_row][command_col].entity = BOULDER; 
                }
                if (player_command == 'g') {
                    board[command_row][command_col].entity = GEM; 
                }
                if (player_command == 'e') {
                    board[command_row][command_col].entity = EXIT_LOCKED; 
                }
                break;
            }
        }
        else {
            printf("Invalid location: position is not on map!\n");
            break;
        }
    }
}

// Illumination command 
void i_direction(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int radius, 
    int lives) {
    if (radius > 0) {
        printf("Illumination Mode: Activated\n");
        illuminate(board, temp, radius, lives);
    }
    else {
        printf("Illumination Mode: Deactivated\n");
    }  
    illuminate(board, temp, radius, lives);
    print_board(temp, player_row(board), player_col(board), lives);  
}

// Dash movement (two command characters at the same time)
void dash_movement(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *breath, 
    char d_one, 
    char d_two, 
    int lives, 
    int *score,
    int *win, 
    int mode) {

    if (*breath != 0) {
        printf("You're out of breath! Skipping dash move...\n");
        *breath = 0;
    } 
    else {
        *breath = *breath + 1;
        // First movement (make sure first movement is valid)
        if (prchk_mv(board, d_one, player_row(board), player_col(board)) == 1) {
            *score += movement(board, d_one, player_row(board), player_col(board), mode);
            copy_board(board, temp);
            if (check_win(board) && *score >= 1000) {
                print_board(temp, player_row(board), player_col(board), lives);
                printf("You Win! Final Score: ");
                printf("%d point(s)!\n", *score - 1000);
                *win = 1;
                return;
            }
            // Second movement
            *score += movement(board, d_two, player_row(board), player_col(board), mode);
            copy_board(board, temp);
            if (check_win(board) && *score >= 1000) {
                print_board(temp, player_row(board), player_col(board), lives);
                printf("You Win! Final Score: ");
                printf("%d point(s)!\n", *score - 1000);
                *win = 1;
                return;
            }
        }
    }
}

// Check if player has lives to continue or not
void check_lives(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int radius, 
    int score, 
    int lives, 
    bool play,
    int counter_lv) {
    // if encounter lava then no chance -> directly end game 
    if (counter_lv == 2) {
        print_board(temp, player_row(board), player_col(board), lives);
        return; 
    }
    if (lives == 0) {
        printf("Game Lost! You scored %d points!\n", score);
        print_board(temp, player_row(board), player_col(board), lives);
        return; 
    }
    if (play == false) {
        printf("Respawn blocked! Game over. Final score: %d points.\n", score);
        illuminate(board, temp, radius, lives);
        print_board(temp, player_row(board), player_col(board), lives);
        return;
    }
}

// Get player current row 
int player_row (struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == PLAYER) {
                return row;
            }
        }
    }
    return 0;    
}

// Get player current col 
int player_col (struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == PLAYER) {
                return col;
            }
        }
    }
    return 0;    
}

// Create a temp to store illumination and lava 
void first_copy_board(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            temp[row][col].entity = board[row][col].entity; 
        }    
    } 
}

// Copy the board to temp
void copy_board(struct tile board[ROWS][COLS], struct tile temp[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (temp[row][col].entity != LAVA) {
                temp[row][col].entity = board[row][col].entity; 
            }
        }    
    } 
}

// Check if the input row/col are in map or not 
bool check_validity (
    struct tile board[ROWS][COLS], 
    int input_row, 
    int input_col) {
    if (input_row < ROWS && input_row >= 0 && 
        input_col < COLS && input_col >= 0) {
        return true;
    }
    return false;
}

// Check command W's validity
bool comw_validity (
    struct tile board[ROWS][COLS], 
    int start_row, 
    int end_row, 
    int start_col, 
    int end_col, 
    int initial_row, 
    int initial_col) {
    for(int i = start_row; i <= end_row; i++) {
        for(int j = start_col; j <= end_col; j++) {
            if (board[i][j].entity != DIRT || 
                (i == initial_row && j == initial_col)) {
                return false;
            }
        }         
    } 
    return true;   
}

// Move the player on the board 
int movement(
    struct tile board[ROWS][COLS], 
    char direction, 
    int player_row, 
    int player_col, 
    int mode) {   
    int result = 0;
    if (direction == 'w' || direction == 'W' ) {
        if (check_movement(board, player_row - 1, player_col)) {
            result = check_collection(board, player_row - 1, player_col, mode);
            board[player_row][player_col].entity = EMPTY;
            board[player_row - 1][player_col].entity = PLAYER;
        }
    }
    if (direction == 'a' ||direction == 'A') {
        if (check_movement(board, player_row, player_col -1)) {
            result = check_collection(board, player_row, player_col -1, mode);
            board[player_row][player_col].entity = EMPTY;            
            board[player_row][player_col - 1].entity = PLAYER;
        }
    }
    if (direction == 's' || direction == 'S') {
        if (check_movement(board, player_row + 1, player_col)) {
            result = check_collection(board, player_row + 1, player_col, mode );
            board[player_row][player_col].entity = EMPTY;
            board[player_row + 1][player_col].entity = PLAYER;
        }
    }
    if (direction == 'd' || direction == 'D') {
        if (check_movement(board, player_row, player_col + 1)) {
            result = check_collection(board, player_row, player_col + 1, mode);
            board[player_row][player_col].entity = EMPTY;
            board[player_row][player_col + 1].entity = PLAYER;
        }
    }
    if (direction == 'r') {
        check_collection(board, player_row - 1, player_col, mode);
        board[player_row][player_col].entity = PLAYER;
    }

    return result; 
}

// Check if the player will encounter W, B EL or not
bool check_movement(struct tile board[ROWS][COLS], int r, int c) {
    if (r < ROWS && r >= 0 && 
        c < COLS && c >= 0) {
        if (board[r][c].entity != WALL && 
            board[r][c].entity != BOULDER && 
            board[r][c].entity != EXIT_LOCKED) {
            return true;
        }
    }
    return false;
}

// Check if after the movement the player is still in map 
int prchk_mv(
    struct tile board[ROWS][COLS], 
    char direction, 
    int player_row, 
    int player_col) {
    if (direction == 'w' || direction == 'W') {
        if (check_movement(board, player_row - 1, player_col)) {
            return 1;
        }
    }
    if (direction == 'a' ||direction == 'A') {
        if (check_movement(board, player_row, player_col -1)) {
            return 1;
        }
    }
    if (direction == 's' || direction == 'S') {
        if (check_movement(board, player_row + 1, player_col)) {
            return 1;
        }
    }
    if (direction == 'd' || direction == 'D') {
        if (check_movement(board, player_row, player_col + 1)) {
            return 1;
        }
    }
    if (direction == 'r') {
        return 1;
    }
    return 0; 
}
 
// count the scores 
int check_collection(
    struct tile board[ROWS][COLS], 
    int r, 
    int c, 
    int mode) {

    // lava mode -> score times 10 
    if (mode != 0) {
        if (board[r][c].entity == GEM) {
            return 200;
        }
        if (board[r][c].entity == DIRT) {
            return 10;
        }
        if (board[r][c].entity == EXIT_UNLOCKED) {
            return 1000;
        }
    }
    else {
        if (board[r][c].entity == GEM) {
            return 20;
        }
        if (board[r][c].entity == DIRT) {
            return 1;
        }
        if (board[r][c].entity == EXIT_UNLOCKED) {
            return 1000;
        }
    }

    return 0;
}

// Get the maximum score that the player can gain 
int get_max_score (struct tile board[ROWS][COLS], int mode) {
    int num_dirts = 0;
    int num_gems = 0;
    int num_boulders = 0;
    int max_score = 0;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == DIRT) {
                num_dirts ++;
            }
            if (board[row][col].entity == GEM) {
                num_gems ++;
            }
            if (board[row][col].entity == BOULDER) {
                num_boulders ++;
            }   
        }
    }

    if (mode == 0) {
        max_score = num_dirts + num_gems * 20;
    }
    // game lava mode on
    else {
        max_score = (num_dirts * 10) + (num_gems * 200);
    }

    return max_score; 
}

// Get number of collection 
int num_clct(struct tile board[ROWS][COLS]) {
    int dirt = 0;
    int gem = 0;
    int total = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == DIRT) {
                dirt ++; 
            }
        }
    }
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == GEM) {
                gem ++;
            }
        }
    }
    total = dirt + gem;
    return total;
}

// Check if the player wins game or not every round 
bool check_win (struct tile board[ROWS][COLS]) {
    bool check = true;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == GEM) {
                return false;
            }
        }
    } 
    // If win then open all the locks 
    if (check == true) {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (board[row][col].entity == EXIT_LOCKED) {
                    board[row][col].entity = EXIT_UNLOCKED;
                }
            }
        }
    }
    int player_r = player_row(board);
    int player_c = player_col(board);
    if (board[player_r][player_c].entity == EXIT_UNLOCKED) {
        return true; 
    }
    return check; 
}

// Print gravity mode 
void printf_gravity(char gravity_direction) {
    if (gravity_direction == 'w') {
        printf("Gravity now pulls UP!\n");
    }
    if (gravity_direction == 's') {
        printf("Gravity now pulls DOWN!\n");
    }
    if (gravity_direction == 'a') {
        printf("Gravity now pulls LEFT!\n");
    }
    if (gravity_direction == 'd') {
        printf("Gravity now pulls RIGHT!\n");
    }
}

// Boulder falling movement 
void boulder_falling(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int *lives, 
    int *irow, 
    int *icol, 
    bool *play, 
    char gravity_direction, 
    int *counter_lv, 
    int score) {

    int shift_x = 0;
    int shift_y = 0;

    if (gravity_direction == 'w') {
        shift_y = -1;
        for (int row = 1; row < ROWS; row++) {  
            for (int col = 0; col < COLS; col++) {
                hit(board, temp, row, col, lives, play, 
                    *irow, *icol, shift_x, shift_y, counter_lv, score);
            }
        }
    }
    else if (gravity_direction == 's') { 
        shift_y = 1;
        for (int row = ROWS - 2; row >= 0; row--) { 
            for (int col = 0; col < COLS; col++) {
                hit(board, temp, row, col, lives, play, 
                    *irow, *icol, shift_x, shift_y, counter_lv, score);
            }
        }
    } 
    else if (gravity_direction == 'a') { 
        shift_x = -1;
        for (int col = 1; col < COLS; col++) { 
            for (int row = 0; row < ROWS; row++) {
                hit(board, temp, row, col, lives, play, 
                    *irow, *icol, shift_x, shift_y, counter_lv, score);
            }
        }
    } 
    else if (gravity_direction == 'd') { 
        shift_x = 1;
        for (int col = COLS - 2; col >= 0; col--) { 
            for (int row = 0; row < ROWS; row++) {
                hit(board, temp, row, col, lives, play, 
                *irow, *icol, shift_x, shift_y, counter_lv, score);
            }
        }
    }
}

// When the boulder hits player 
void hit(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int row, 
    int col, 
    int *lives, 
    bool *play, 
    int irow, 
    int icol, 
    int shift_x, 
    int shift_y, 
    int *counter_lv,  
    int score) {

    if (board[row][col].entity == BOULDER && 
        board[row + shift_y][col + shift_x].entity == PLAYER) {
        (*lives) -= 1;

        board[row][col].entity = EMPTY;

        // If no lives then end game directly 
        if (*lives == 0) {
            *play = false;  
            return;         
        }

        // Check if can respawn 
        if (temp[irow][icol].entity == LAVA) {
            printf("Respawn blocked! ");
            printf("You're toast! Final score: %d points.\n", score);
            *play = 0; 
            *counter_lv = 2;
            return;
        }
        else if (check_movement(board, irow, icol)) {
            printf("Respawning!\n");
            board[irow][icol].entity = PLAYER;  
            board[row + shift_y][col + shift_x].entity = BOULDER;
        } 
        // No respawn possible, game over
        else {
            *play = false;  
        }
    }
    if (board[row][col].entity == BOULDER && 
        board[row + shift_y][col + shift_x].entity == EMPTY) {
        board[row][col].entity = EMPTY;
        board[row + shift_y][col + shift_x].entity = BOULDER;
    }
}

// Illumination mode on 
void illuminate(
    struct tile board[ROWS][COLS], 
    struct tile temp[ROWS][COLS], 
    int radius, 
    int lives) {
    int ply_row = player_row(board);
    int ply_col = player_col(board);
    double distance;


    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            int temp_row = (row - ply_row) * (row - ply_row);
            int temp_col = (col - ply_col) * (col - ply_col);

            distance = sqrt(temp_row + temp_col);
            if (temp[row][col].entity != LAVA && 
                distance > radius && radius > 0) {
                temp[row][col].entity = HIDDEN; 
            }
        }
    }
}


// =============================================================================
// Definitions of Provided Functions
// =============================================================================

// Given a 2D board array, initialise all tile entities to DIRT.
void initialise_board(struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col].entity = DIRT;
        }
    }
}

// Prints the game board, showing the player's position and lives remaining
void print_board(
    struct tile board[ROWS][COLS], 
    int player_row, 
    int player_col,
    int lives_remaining
) {
    print_board_line();
    print_board_header(lives_remaining);
    print_board_line();
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            printf("|");
            if (row == player_row && col == player_col) {
                printf("^_^");
                board[row][col].entity = PLAYER;
            } else if (board[row][col].entity == EMPTY) {
                printf("   ");
            } else if (board[row][col].entity == DIRT) {
                printf(" . ");
            } else if (board[row][col].entity == WALL) {
                printf("|||");
            } else if (board[row][col].entity == BOULDER) {
                printf("(O)");
            } else if (board[row][col].entity == GEM) {
                printf("*^*");
            } else if (board[row][col].entity == EXIT_LOCKED) {
                printf("[X]");
            } else if (board[row][col].entity == EXIT_UNLOCKED) {
                printf("[ ]");
            } else if (board[row][col].entity == HIDDEN) {
                printf(" X ");
            } else if (board[row][col].entity == LAVA) {
                printf("^^^");
            } else {
                printf("   ");
            }
        }
        printf("|\n");
        print_board_line();
    }
    printf("\n");
    return;
}

// Helper function for print_board(). You will not need to call this.
void print_board_header(int lives) {
    printf("| Lives: %d    C A V E R U N             |\n", lives);
}

// Helper function for print_board(). You will not need to call this.
void print_board_line(void) {
    printf("+");
    for (int col = 0; col < COLS; col++) {
        printf("---+");
    }
    printf("\n");
}

// Prints game statistics: tile types, completion %, and points remaining.
void prt_map_stcs(
    struct tile board[ROWS][COLS], 
    int current,
    int max, 
    int mode) {

    int number_of_dirt_tiles = 0;
    int number_of_gem_tiles = 0;
    int number_of_boulder_tiles = 0;
    double completion_percentage;
    int maximum_points_remaining;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == DIRT) {
                number_of_dirt_tiles ++;
            }
            if (board[row][col].entity == GEM) {
                number_of_gem_tiles ++;
            }
            if (board[row][col].entity == BOULDER) {
                number_of_boulder_tiles ++;
            }   
        }
    }

    maximum_points_remaining = get_max_score(board, mode); 
    completion_percentage = (double) current / (double) max * 100; 

    printf("========= Map Statistics =========\n");
    printf("Tiles Remaining by Type:\n");
    printf("  - DIRT:      %d\n", number_of_dirt_tiles);
    printf("  - GEMS:      %d\n", number_of_gem_tiles);
    printf("  - BOULDERS:  %d\n", number_of_boulder_tiles);
    printf("Completion Status:\n");
    printf("  - Collectible Completion: %.1f%%\n", completion_percentage);
    printf("  - Maximum Points Remaining: %d\n", maximum_points_remaining);
    printf("==================================\n");
}