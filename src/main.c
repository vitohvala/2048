/*
 * 
 *
 * */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include "term_config.h"

/*
 * 
 * 
 * */
#define SIZE 4
#define SPACES 7
#define FG "[38;5;255;"
#define BG "48;5;"

uint8_t pick_number(){
    int random_number = rand()%10;
    return (random_number > 7) ? 4 : 2;
}
uint8_t get_digit_count(uint16_t num){
    uint8_t count = 1;
    while(num > 9){
        count++;
        num /= 10;
    }
    return count;
}


void print_number(uint16_t num, uint8_t spaces){

    for(uint8_t i = 0; i < spaces / 2; i++)
        printf(" ");
    printf("%u", num);
    if(spaces % 2) spaces++;
    for(uint8_t i = 0; i < spaces / 2; i++)
        printf(" ");
}
void print_block(uint16_t num, uint8_t *full, uint16_t *max){
    if(num > 0) {
        if((*max) < num) (*max) = num;
        (*full)++;
        print_number(num, (SPACES - get_digit_count(num)));
    }else printf("   ·   ");

}

void draw_board(uint16_t matrix[SIZE][SIZE], uint8_t *full, uint16_t *max){

    *full = 0;
    uint8_t bg;


    for(int i = 0; i < SIZE; i++){
        for(int k = 0; k < SIZE - 1; k++){
            for(int j = 0; j < SIZE; j++){

                bg = matrix[i][j] % 255;
                printf("\033%s%s%um", FG, BG, bg);

                if(k == 1) print_block(matrix[i][j], full, max);
                else printf("       ");
            }
            printf("\033[m\n");

        }
    }

}

void put_number(uint16_t matrix[SIZE][SIZE]){
    int x, y;
    do {
        x = rand() % 4;
        y = rand() % 4;
    } while(matrix[x][y] != 0);

    matrix[x][y] = pick_number();
}



uint8_t slide_count_right(uint16_t matrix[][SIZE], int x, int y){
    int count = 0;
    for(int j = y + 1; j < SIZE; j++){
        if(matrix[x][j] == 0) count++;
        else break;
    }
    return count;
}

bool slide_right(uint16_t matrix[][SIZE]){
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

uint8_t slide_count_down(uint16_t matrix[][SIZE], int x, int y){
    int count = 0;
    for(int i = x + 1; i < SIZE; i++){
        if(matrix[i][y] == 0) count++;
        else break;
    }
    return count;
}

bool slide_down(uint16_t matrix[][SIZE]){
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
uint8_t slide_count_left(uint16_t matrix[][SIZE], int x, int y){
    int count = 0;
    for(int j = y - 1; j >= 0; j--){
        if(matrix[x][j] == 0) count++;
        else break;
    }
    return count;
}
bool slide_left(uint16_t matrix[][SIZE]){
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
uint8_t slide_count_up(uint16_t matrix[][SIZE], int x, int y){
    int count = 0;
    for(int i = x - 1; i >= 0; i--){
        if(matrix[i][y] == 0) count++;
        else break;
    }
    return count;
}
bool slide_up(uint16_t matrix[][SIZE]){
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

bool check_end(uint16_t matrix[][SIZE]){
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
uint8_t input_cntrl(uint16_t matrix[][SIZE]){
    char input = getchar();
    uint8_t succes = false;


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
            succes = 2;
            break;
    }
    return succes;
}
void init(uint16_t matrix[SIZE][SIZE]){
    memset(matrix, 0, SIZE * SIZE * sizeof(uint16_t));
    
    clear();
    terminal_config();
    int randomx = rand()%SIZE;
    int randomy = rand()%SIZE;
    matrix[randomx][randomy] = pick_number();
}
/*
 * main
 *
 * */
int main(int argc, char**argv){
    (void)argc; (void)argv;

    uint8_t status = 1;
    uint16_t matrix[SIZE][SIZE];
    uint8_t succes = 0;
    uint8_t full = 0;
    uint16_t max = 0;

    srand(time(0));
    init(matrix);
    draw_board(matrix, &full, &max); 
    printf("w,a,s,d to move, q to quit!\n");

    while(status){
        succes = input_cntrl(matrix);
        if(succes == 1){
            clear();
            put_number(matrix);
            draw_board(matrix, &full, &max);
            printf("w,a,s,d to move, q to quit!\n");
            succes = 0;
        }else if(succes == 2){
            clear();
            draw_board(matrix, &full, &max);
            printf("Are you sure? [y/N]\n");
            char inp = getchar();
            if(inp == 'Y' || inp == 'y') {
                clear();
                draw_board(matrix, &full, &max);
                status = 0;
            }else {
                clear();
                draw_board(matrix, &full, &max);
                printf("w,a,s,d to move, q to quit!\n");
            }
            succes = 0;

        }
        if(full >= 16) status = check_end(matrix);
        if(max == 2048) status = 0;
    }

    printf("score [%u]\n", max);
    return 0;
}
