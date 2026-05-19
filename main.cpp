#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h> 

using namespace std;

#define ITALIC "\033[3m" 
#define RESET "\033[0m"  
#define BOLD "\033[1m"   
#define PINK "\033[35m"  
#define RED "\033[31m"   
#define CROSSED "\033[9m"

#define MAX_STRING_LEN 100
#define SAVE_FILE "savegame.txt"

struct Coordinate{
    int x;
    int y;
};

int open_main_menu();
int open_settings_menu();
int open_game_menu();
void open_ending_screen(char symm, int draw);

char** generate_empty_board(int size);
void print_board(int board_size, char** board);
int check_win(char** board, int size, int x, int y, char sym);
void start_pvp_game(char** board, int board_size, char* input_buffer, char* helper, int start_turn);

Coordinate bot_move_random(char** board, int board_size);
void start_pve_game(char** board, int board_size, char* input_buffer, char* helper, int start_turn);
int calculate_line_score(char** board, int size, int x, int y, char sym);
int evaluate_move(char** board, int size, int x, int y);
Coordinate bot_move_smart(char** board, int board_size);

void decapitalize_word(char* word);

void save_file(int mode, int turn, int board_size, char** board);
void load_file(int& mode, int& turn, int& board_size, char**& board);
int has_saved_moves(const char* filename);



int main()
{
    srand(time(NULL));
    system("cls");

    int board_size = 15;
    
    int menu_choice;
    int settings_choice;
    int game_choice;

    char input_buffer[MAX_STRING_LEN];
    char helper[MAX_STRING_LEN];
    
    char** board = generate_empty_board(board_size);

    do{
        system("cls");
        menu_choice = open_main_menu();

        if(menu_choice == 1){ 
            system("cls");

            if (!has_saved_moves(SAVE_FILE)) { 
                cout << RED << "There is no saved game.\n" << RESET;
                cout << ITALIC << "Press Enter to return to the menu\n" << RESET;
                fgets(helper, 256, stdin);
            }
            else {
                int saved_mode = 0, saved_turn = 0;
                load_file(saved_mode, saved_turn, board_size, board); 
                
                cout << BOLD << PINK << "Game loaded successfully!\n" << RESET;
                cout << ITALIC << "Press Enter to resume playing...\n" << RESET;
                fgets(helper, 256, stdin);
                
                if (saved_mode == 1){
                    start_pvp_game(board, board_size, input_buffer, helper, saved_turn);
                } 
                else if (saved_mode == 2){
                    start_pve_game(board, board_size, input_buffer, helper, saved_turn);
                }
            }
        }
        else if(menu_choice == 2){
            do{
                system("cls");
                game_choice = open_game_menu();

                if (game_choice == 1){
                    for(int i = 0; i < board_size; i++) free(board[i]);
                    free(board);
                    
                    board = generate_empty_board(board_size);
                    start_pvp_game(board, board_size, input_buffer, helper, 0);
                } 
                else if (game_choice == 2){
                    for(int i = 0; i < board_size; i++) free(board[i]);
                    free(board);
                    
                    board = generate_empty_board(board_size);
                    start_pve_game(board, board_size, input_buffer, helper, 0);
                }

            }while(game_choice != 3);
        }
        else if(menu_choice == 3){

            do{
                system("cls");
                settings_choice = open_settings_menu();

                if (settings_choice == 1) {
                    system("cls");
                    int newSize;
                    int input_status;

                    while(1){
                        printf("Enter your board size (min: 10, max: 70). Default: 15\n");
                        cout << "Current board size: " << board_size << "\n";
                        
                        input_status = scanf("%d", &newSize);
                        while(getchar() != '\n' && !feof(stdin));

                        if(input_status == 1 && newSize >= 10 && newSize <= 70){
                            for(int i = 0; i < board_size; i++) free(board[i]);
                            free(board);
                            
                            board_size = newSize;
                            board = generate_empty_board(board_size); 
                            printf("\nBoard size set to: %d\n", board_size);
                            break;
                        }

                        system("cls");
                        printf(RED "Invalid board size. Try again.\n\n" RESET);
                    }
                    
                    cout << ITALIC << "Press Enter to exit\n" << RESET;
                    fgets(helper, 256, stdin); 
                }
            }while(settings_choice != 2);
        }
        else if(menu_choice == 4){
            system("cls");
            cout << "Two players take turns placing symbols (O or X) on a grid, aiming to be\nthe first to align five in a row horizontally, vertically or diagonally\n" << endl;
            cout << ITALIC << "Press Enter to exit\n" << RESET;
            fgets(helper, 256, stdin);
        }
        else if(menu_choice == 5){
            system("cls");
            cout << "Application developed by Oleksandr Melnychenko, an IASA KPI student from group KI-52 || KYIV 2026\n" << endl;
            cout << ITALIC << "Press Enter to exit\n" << RESET;
            fgets(helper, 256, stdin);
        }

    }while(menu_choice!=6);

    for(int i = 0; i < board_size; i++) free(board[i]); 
    free(board);
    
    return 0;
}



int open_main_menu()
{
    int menu_choice;
    int input_status;

    while (1){
        cout << PINK << BOLD << "\nGOMOKU\n" << RESET;
        cout << PINK << BOLD << "\nMAIN MENU\n" << RESET;
        printf("==============================\n");
        if(has_saved_moves(SAVE_FILE)) printf("* 1. Continue\n");
        else printf("* " CROSSED "1. Continue\n" RESET);
        printf("* 2. New Game\n");
        printf("* 3. Settings\n");
        printf("* 4. Rules\n");
        printf("* 5. Credits\n\n");
        cout << RED << "* 6. Quit\n" << RESET;
        printf("==============================\n");
        printf("Enter your choice (1-6): ");

        input_status = scanf("%d", &menu_choice);
        while (getchar() != '\n' && !feof(stdin));

        if(input_status == 1 && menu_choice >= 1 && menu_choice <= 6) 
        {
            return menu_choice;
        }

        system("cls");
        cout << RED << "Invalid choice. Try again\n" << RESET;
    }
}

int open_settings_menu(){
    int menu_choice;
    int input_status;

    while (1){
        printf("\n\nSETTINGS\n");
        printf("------------------------------\n");
        printf("* 1. Board size\n");
        printf("* 2. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice (1-2): ");

        input_status = scanf("%d", &menu_choice);
        while (getchar() != '\n' && !feof(stdin));

        if(input_status == 1 && menu_choice >= 1 && menu_choice <= 2) 
        {
            return menu_choice;
        }

        system("cls");
        cout << RED << "Invalid choice. Try again\n" << RESET;
    }
}

int open_game_menu(){
    int menu_choice;
    int input_status;

    while (1){
        printf("\n\nNEW GAME\n");
        printf("------------------------------\n");
        printf("* 1. Player vs Player\n");
        printf("* 2. Player vs Bot\n");
        printf("* 3. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice (1-3): ");

        input_status = scanf("%d", &menu_choice);
        while (getchar() != '\n' && !feof(stdin));

        if(input_status == 1 && menu_choice >= 1 && menu_choice <= 3) 
        {
            return menu_choice;
        }

        system("cls");
        cout << RED << "Invalid choice. Try again\n" << RESET;
    }
}

void open_ending_screen(char sym, int draw){
    if(sym != 'B' && !draw){
        int winner = (sym == 'X') ? 1 : 0;
        
        cout << "===========================================\n";
        cout << BOLD << PINK << "Congratulation! Player " << winner << " has won!\n" << RESET;
        cout << ITALIC << "Press Enter to exit\n" << RESET;
        cout << "===========================================\n";
    }
    else if(sym == 'B' && !draw){
        cout << "===========================================\n";
        cout << BOLD << RED << "YOU'VE LOST\n" << RESET;
        cout << BOLD << "Better luck next time\n\n" << RESET;
        cout << ITALIC << "Press Enter to exit\n" << RESET;
        cout << "===========================================\n";
    }
    else if(draw){
        cout << "===========================================\n";
        cout << BOLD << PINK << "The game ended in a draw!\n" << RESET;
        cout << ITALIC << "Press Enter to exit\n" << RESET;
        cout << "===========================================\n";
    }

    return;
}

char** generate_empty_board(int size){
    char** board = (char**)malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
    }

    for(int i = 0; i < size; i++){
        for (int j = 0; j < size; j++) board[i][j] = '.';
    }

    return board;
}

void print_board(int board_size, char** board){
    printf("   ");
    for(int i = 0; i < board_size; i++)
        printf(BOLD PINK "%2d " RESET, i+1);

    printf("\n");

    for (int i = 0; i < board_size; i++){
        printf(BOLD PINK "%2d " RESET, i+1); 
        
        for (int j = 0; j < board_size; j++) 
            printf(" %c ", board[i][j]);
            
        printf("\n");
    }
}

int check_win(char** board, int size, int x, int y, char sym){

    int directions[4][2] = {{0, 1},{ 1, 0}, {1, 1}, {1, -1}};

    for(int i = 0; i < 4; i++){
        int count = 1; 
        int dx = directions[i][0];
        int dy = directions[i][1];

        int r = x + dx, c = y + dy; 
        while(r >= 0 && r < size && c >= 0 && c < size && board[r][c] == sym){
            count++;
            r += dx;
            c += dy;
        }

        r = x - dx; c = y - dy;
        while(r >= 0 && r < size && c >= 0 && c < size && board[r][c] == sym){
            count++;
            r -= dx;
            c -= dy;
        }

        if(count >= 5) return 1; 
    }

    return 0;
}

void start_pvp_game(char** board, int board_size, char* input_buffer, char* helper, int start_turn){
    system("cls");
    int x = 0, y = 0;
    
    int turn = start_turn; 
    while(1){
        system("cls"); 
        print_board(board_size, board);

        if(turn % 2 == 0) cout << "Player 1 [X]: Enter coordinates (row column) to place your mark:\n";
        else cout << "Player 2 [O]: Enter coordinates (row column) to place your mark:\n";
        cout << ITALIC << "Type \"exit\" to leave current game\n\n" << RESET;    

        if (fgets(input_buffer, 256, stdin) == NULL) break;
        printf("\n");

        input_buffer[strcspn(input_buffer, "\n")] = 0;
        decapitalize_word(input_buffer);

        if (strcmp(input_buffer, "exit") == 0) {
            break; 
        }

        int input_status = sscanf(input_buffer, "%d %d", &x, &y);
        
        if(input_status != 2){
            printf(RED "Error: Please enter two numbers separated by a space.\n" RESET);
            cout << ITALIC << "Press Enter to continue\n" << RESET;
            fgets(helper, 256, stdin); 
            continue; 
        }

        printf("\n\n");
        x--;
        y--;

        if(x < 0 || x >= board_size || y < 0 || y >= board_size || board[x][y] != '.'){
            cout << RED << "Invalid coordinates or the position is already taken, try again\n" << RESET;
            cout << ITALIC << "Press Enter to continue\n" << RESET;
            fgets(helper, 256, stdin);
        }
        else{
            char sym = (turn % 2 == 0) ? 'X' : 'O';

            board[x][y] = sym;

            save_file(1, turn + 1, board_size, board);

            if(check_win(board, board_size, x, y, sym)){
                system("cls"); 
                print_board(board_size, board);
                printf("\n\n");

                open_ending_screen(sym, 0);
                remove(SAVE_FILE);
                
                fgets(helper, 256, stdin);
                break;
            }

            if(turn == board_size*board_size-1){
                system("cls"); 
                print_board(board_size, board);
                printf("\n\n");

                open_ending_screen(sym, 1);
                remove(SAVE_FILE);

                fgets(helper, 256, stdin);
                break;
            }

            turn++;
        }
    }
}

void start_pve_game(char** board, int board_size, char* input_buffer, char* helper, int start_turn){
    system("cls");
    
    int x = 0, y = 0;
    int turn = start_turn; 
    while(1){
        system("cls"); 
        print_board(board_size, board);

        if(turn % 2 == 0){
            cout << "Player 1 : Enter coordinates (row column) to place your mark [X]:\n";
        
            cout << ITALIC << "Type \"exit\" to leave current game\n\n" << RESET;    

            if (fgets(input_buffer, 256, stdin) == NULL) break;
            printf("\n");

            input_buffer[strcspn(input_buffer, "\n")] = 0;
            decapitalize_word(input_buffer);

            if(strcmp(input_buffer, "exit") == 0){
                break; 
            }

            int input_status = sscanf(input_buffer, "%d %d", &x, &y);
            
            if(input_status != 2){
                printf(RED "Error: Please enter two numbers separated by a space.\n" RESET);
                cout << ITALIC << "Press Enter to continue\n" << RESET;
                fgets(helper, 256, stdin); 
                continue; 
            }

            printf("\n\n");
            x--;
            y--;

            if(x < 0 || x >= board_size || y < 0 || y >= board_size || board[x][y] != '.'){
                cout << RED << "Invalid coordinates or the position is already taken, try again\n" << RESET;
                cout << ITALIC << "Press Enter to continue\n" << RESET;
                fgets(helper, 256, stdin);
                continue;
            }
        }
        else{
           Coordinate bot_move = bot_move_smart(board, board_size);
           
           x = bot_move.x;
           y = bot_move.y;
        }

        char sym = (turn % 2 == 0) ? 'X' : 'O';
        
        board[x][y] = sym;

        save_file(2, turn + 1, board_size, board);

        if(check_win(board, board_size, x, y, sym)){
            system("cls"); 
            print_board(board_size, board);
            printf("\n\n");

            if(turn % 2 == 1) sym = 'B';
            open_ending_screen(sym, 0);

            remove(SAVE_FILE);

            fgets(helper, 256, stdin);
            break;
        }
        
        if(turn == board_size*board_size-1){
            system("cls"); 
            print_board(board_size, board);
            printf("\n\n");
            
            open_ending_screen(sym, 1);

            remove(SAVE_FILE);

            fgets(helper, 256, stdin);
            break;
        }

        turn++;
    }
}

void decapitalize_word(char* word){
    for(int i = 0; word[i] != '\0'; i++){
        if(word[i] >= 'A' && word[i] <= 'Z'){
            word[i] += 32;
        }
    }
}

void save_file(int mode, int turn, int board_size, char** board){
    FILE* file = fopen(SAVE_FILE, "w");
    if(file != NULL){
        fprintf(file, "%d\n%d\n%d\n", mode, turn, board_size);
        
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                fprintf(file, "%c", board[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
}

void load_file(int& mode, int& turn, int& board_size, char**& board){
    FILE* file = fopen(SAVE_FILE, "r");
    if(file != NULL){
        int saved_size;
        
        fscanf(file, "%d\n%d\n%d", &mode, &turn, &saved_size);

        if (saved_size != board_size) {
            for(int i = 0; i < board_size; i++) free(board[i]);
            free(board);
            
            board_size = saved_size;
            board = generate_empty_board(board_size);
        }

        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                fscanf(file, " %c", &board[i][j]); 
            }
        }
        fclose(file);
    }
}

int has_saved_moves(const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL) return 0; 

    int m, t, s; 
    if(fscanf(file, "%d %d %d", &m, &t, &s) != 3){
        fclose(file);
        return 0; 
    }

    char c;
    while(fscanf(file, " %c", &c) == 1){
        if(c == 'X' || c == 'O' || c == 'B'){ 
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}



int calculate_line_score(char** board, int size, int x, int y, char sym){
    int total_score = 0;
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for(int i = 0; i < 4; i++){
        int count = 1; 
        int dx = directions[i][0];
        int dy = directions[i][1];

        int r = x + dx, c = y + dy; 
        while(r >= 0 && r < size && c >= 0 && c < size && board[r][c] == sym){
            count++;
            r += dx;
            c += dy;
        }

        r = x - dx; c = y - dy;
        while(r >= 0 && r < size && c >= 0 && c < size && board[r][c] == sym){
            count++;
            r -= dx;
            c -= dy;
        }

        if(count >= 5) total_score += 100000;
        else if(count == 4) total_score += 10000;
        else if(count == 3) total_score += 1000;
        else if(count == 2) total_score += 10;
    }

    return total_score;
}

int evaluate_move(char** board, int size, int x, int y){
    int score = 0;

    score += calculate_line_score(board, size, x, y, 'O');
    score += calculate_line_score(board, size, x, y, 'X'); 

    return score;
}

Coordinate bot_move_random(char** board, int board_size){
    while(1){
        int randx = rand() % board_size;
        int randy = rand() % board_size;

        if(board[randx][randy] == '.') {
            board[randx][randy] = 'O';
            return {randx, randy};
        }
    }
}

Coordinate bot_move_smart(char** board, int board_size){
    int best_score = -1;
    Coordinate best_move = {-1, -1};

    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            
            if(board[i][j] == '.'){
                
                int current_score = evaluate_move(board, board_size, i, j);
                
                if(current_score > best_score){
                    best_score = current_score;
                    best_move = {i, j};
                }
            }
        }
    }

    if(best_score == 0){
          bot_move_random(board, board_size);
    }

    board[best_move.x][best_move.y] = 'O';
    return best_move;
}
