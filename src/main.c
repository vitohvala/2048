/*
 * 
 *
 * */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>

typedef struct{
    int x;
    int y;
}EMPTY;

/*
 * 
 * 
 * */
#define SIZE 4

static struct termios og_t;

void clear(void){
    printf("\x1b[2J");
    printf("\x1b[H");
}

void reset_terminal(void){
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &og_t);
    printf("\x1b[?25h");
}

void terminal_config(void){
    int res = 0;
    printf("\x1b[?25l");
    res = tcgetattr(STDIN_FILENO, &og_t);
    assert(res == 0);
    struct termios new_termios = og_t;
 
    new_termios.c_iflag &= ~(IXON);
    new_termios.c_lflag &= ~(ICANON | ECHO| ISIG);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] =  1;

    res = tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    assert(res == 0);

    atexit(reset_terminal);
}

void get_empty_pos(int matrix[SIZE][SIZE], EMPTY arr[], int *arr_size){
    *arr_size = 0;
    for(int i = 0 ; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(!matrix[i][j]){
                arr[(*arr_size)].x = i;
                arr[(*arr_size)].y = j;
                (*arr_size)++;
            }
        }
    }
}

int pick_number(){
    int random_number = rand()%10;
    return (random_number > 7) ? 4 : 2;
}

void draw_board(int matrix[SIZE][SIZE]){
    printf("________________________\n");

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            printf("|");
            if(matrix[i][j] < 10 && matrix[i][j])
                printf("  %d  ", matrix[i][j]);
            else if(matrix[i][j] > 10 && matrix[i][j] < 100)
                printf(" %d  ", matrix[i][j]);
            else if(matrix[i][j] < 1000 && matrix[i][j] > 100)
                printf(" %d ", matrix[i][j]);
            else printf("     ");
        }
        printf("|\n");
        printf("------------------------\n");
    }
}
void swap(EMPTY empty_pos[], int pos, int pos2){
    int x = empty_pos[pos].x, y = empty_pos[pos].y;
    empty_pos[pos] = empty_pos[pos2];
    empty_pos[pos2].x = x;
    empty_pos[pos2].y = y;
}

void put_number(EMPTY empty_pos[], int *arr_size, int matrix[SIZE][SIZE]){
    int random_number = rand()%(*arr_size);
    int x, y;
    x = empty_pos[random_number].x;
    y = empty_pos[random_number].y;
    
    swap(empty_pos, random_number, *arr_size - 1);

    (*arr_size)--;
    matrix[x][y] = pick_number();  
}

void init(int matrix[SIZE][SIZE]){
    int randomx = rand()%SIZE;
    int randomy = rand()%SIZE;
    matrix[randomx][randomy] = pick_number();
}

int slide_count_right(int matrix[][SIZE], int x, int y){
    int count = 0;
    for(int j = y + 1; j < SIZE; j++){
        if(matrix[x][j] == 0) count++;
        else break;
    }
    return count;
}

int slide_right(int matrix[][SIZE]){
    int ret = 0, yh = 0, checked = -1;
    for(int i = 0; i < SIZE; i++){
        checked = -1;
        for(int j = SIZE - 2; j >= 0; j--){
            if(matrix[i][j]){

                yh = slide_count_right(matrix, i, j) + j;
                if(yh && yh != j){
                    ret = (ret < 2) ? 1 : ret;
                    matrix[i][yh] = matrix[i][j];
                    matrix[i][j] = 0;
                    j = yh;
                
                }
                if(matrix[i][j] == matrix[i][j + 1] && j < SIZE - 1 && (j + 1) != checked){
                    matrix[i][j + 1] *= 2;
                    ret = matrix[i][j + 1];
                    matrix[i][j] = 0;
                    checked = j + 1;
                }
            }
        }
 
    }
    return ret;
}

int slide_count_down(int matrix[][SIZE], int x, int y){
    int count = 0;
    for(int i = x + 1; i < SIZE; i++){
        if(matrix[i][y] == 0) count++;
        else break;
    }
    return count;
}

int slide_down(int matrix[][SIZE]){
    int ret = 0, xh = 0, checked = -1;
    for(int j = 0; j < SIZE; j++){
        checked = -1;
        for(int i = SIZE - 2; i >= 0; i--){
            if(matrix[i][j]){

                xh = slide_count_down(matrix, i, j) + i;
                if(xh && xh != i){
                    ret = (ret < 2) ? 1 : ret;
                    matrix[xh][j] = matrix[i][j];
                    matrix[i][j] = 0;
                    i = xh;
                }       
                if(matrix[i][j] == matrix[i + 1][j] && i < SIZE - 1 && (i + 1) != checked){
                    matrix[i + 1][j] *= 2;
                    ret = matrix[i + 1][j];
                    matrix[i][j] = 0;
                    checked = i + 1;
                }
            }
        }              

    }
    return ret;
}
int slide_count_left(int matrix[][SIZE], int x, int y){
    int count = 0;
    for(int j = y - 1; j >= 0; j--){
        if(matrix[x][j] == 0) count++;
        else break;
    }
    return count;
}
int slide_left(int matrix[][SIZE]){
    int ret = 0, yh = SIZE - 1, checked = -1;
    for(int i = 0; i < SIZE; i++){
        checked = -1;
        for(int j = 1; j < SIZE; j++){
            if(matrix[i][j]){

                yh = j - slide_count_left(matrix, i, j);
                if(yh != SIZE - 1 && yh != j){
                    ret = (ret < 2) ? 1 : ret;
                    matrix[i][yh] = matrix[i][j];
                    matrix[i][j] = 0;
                    j = yh;
                }
                if(matrix[i][j - 1] == matrix[i][j] && j > 0 && checked != (j - 1)){
                    matrix[i][j - 1] *= 2;
                    ret = matrix[i][j - 1];
                    matrix[i][j] = 0;
                    checked = j - 1;
                }
            }
        }

    }

    return ret;
}
int slide_count_up(int matrix[][SIZE], int x, int y){
    int count = 0;
    for(int i = x - 1; i >= 0; i--){
        if(matrix[i][y] == 0) count++;
        else break;
    }
    return count;
}
int slide_up(int matrix[][SIZE]){
    int ret = 0, xh = SIZE - 1, checked = - 1;
    for(int j = 0; j < SIZE; j++){
        checked = -1;
        for(int i = 1; i < SIZE; i++){
            if(matrix[i][j]){
                xh = i - slide_count_up(matrix, i, j);
                if(xh != SIZE - 1 && xh != i){
                    ret = (ret < 2) ? 1 : ret;
                    matrix[xh][j] = matrix[i][j];
                    matrix[i][j] = 0;
                    i = xh;
                }
                if(matrix[i - 1][j] == matrix[i][j] && i > 0 && (i - 1) != checked){
                    matrix[i - 1][j] *= 2;
                    ret = matrix[i - 1][j];
                    matrix[i][j] = 0;
                    checked = i - 1;
                }
            }
        }

    }

    return ret;
}

int check_end(int matrix[][SIZE]){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(i < SIZE - 1){
                if(matrix[i + 1][j] == matrix[i][j]) return 1;
            } 
            if(j < SIZE - 1){
                if(matrix[i][j + 1] == matrix[i][j]) return 1;
            }

        }
    }
    return 0;
}
/*
 * main
 *
 * */
int main(int argc, char**argv){
    (void)argc; (void)argv;

    int empty_size = 0, end = 1, max = 0;
    int matrix[SIZE][SIZE];
    EMPTY empty_pos[(SIZE * SIZE)];
    char input;
    int succes = 0;

    memset(matrix, 0, SIZE * SIZE * sizeof(int));

    clear();
    srand(time(0));
    init(matrix);
    terminal_config();
    get_empty_pos(matrix, empty_pos, &empty_size);
    
    draw_board(matrix); 

    while(end){
        input = getchar();
        switch(input){
            case 'd':
            case 'D':
                succes = slide_right(matrix);
                break;
            case 'a':
            case 'A':
                succes = slide_left(matrix);
                break;
            case 's':
            case 'S':
                succes = slide_down(matrix);
                break;
            case 'w':
            case 'W':
                succes = slide_up(matrix);
                break;
            case 'Q':
            case 'q':
                end = 0;
        }
        
        if(succes > max) max = succes;
        if(succes > 0){
            get_empty_pos(matrix, empty_pos, &empty_size);
            put_number(empty_pos, &empty_size, matrix);
            clear();
            draw_board(matrix);
            succes = 0;
        }
        if(!empty_size) end = check_end(matrix);
        if(max == 2048) end = 0;
    }

    printf("%d\n", max);
    return 0;
}
