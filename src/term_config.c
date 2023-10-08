#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


static struct termios og_t;

void clear(void){
    printf("\x1b[2J"); //clears the screen
    printf("\x1b[H"); //set cursor position to 0, 0
}

/*
 * flush 
 * reset all terminal changes to original state
 * reset color changes
 */

void reset_terminal(void){
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &og_t);
    printf("\x1b[?25h");
}

/*
 * get the old config and save it
 * disable ctrl-q, ctrl-c...etc signals
 * reset terminal at exit
 * */

void terminal_config(void){
    int res = 0;
    printf("\x1b[?25l");
    res = tcgetattr(STDIN_FILENO, &og_t);
    assert(res == 0);
    struct termios new_termios = og_t;
 
    new_termios.c_iflag &= ~(IXON);//disable ctrl-q, ctrl-s
    //ISIG disable ctrl-c ctrl-z, disable SIGINT, SIGSTP signals 
    //ECHO disables echo in this case
    //and ICANON sets canonical mode
    new_termios.c_lflag &= ~(ICANON | ECHO| ISIG);
    new_termios.c_cc[VMIN] = 1; //minimum number of characters to wait
    new_termios.c_cc[VTIME] =  1; //minimum time to wait 1=0.1

    res = tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    assert(res == 0);

    atexit(reset_terminal);
}

