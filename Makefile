CC=cc
CFLAGS=-Wall -g -Wextra -Werror -std=c99 -pedantic -Wno-deprecated-declarations -Os
LIBS=src/term_config
SRC=src/main.c $(LIBS).o
BIN=2048
all: $(SRC)
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)

$(LIBS).o:
	$(CC) -c $(LIBS).c -o $(LIBS).o

clean:
	rm -f $(BIN) $(LIBS).o
