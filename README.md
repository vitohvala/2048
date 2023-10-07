# 2048
Ah yes another console version of the game 2048;

### Requirements:
  * C compiler
  * make(optional)

### Run 
```console
  $ git clone https://github.com/vitohvala/2048.git
  $ cd 2048
  $ make
  $ ./2048
```
#### OR:
```console
  $ gcc -c src/term_config.c -o src/term_config.o
  $ gcc src/main.c src/temp_config.o -o 2048
  $ ./2048
```
