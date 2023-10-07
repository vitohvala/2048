# 2048
Ah yes another console version of the game 2048;

I stole(or should I say "I was inspired by"?) the design from [2048.c](https://github.com/mevdschee/2048.c)

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
