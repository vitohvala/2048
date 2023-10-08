#ifndef _TERM_CONFIG_H_
#define _TERM_CONFIG_H_


//clears the screen
//and set the cursor position to 0, 0
void clear(void);

//reset terminal to original config
void reset_terminal(void);

//configure terminal
//turn off echo and canonical-mode
//disable ctrl-q, ctrl-c, ctrl-z, ctrl-v signals
void terminal_config(void);

#endif
