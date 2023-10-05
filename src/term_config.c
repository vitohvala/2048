#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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

