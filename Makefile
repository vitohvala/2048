CC=cc
CFLAGS=-Wall -g -Wextra -Werror -std=c99 -pedantic -Wno-deprecated-declarations -Os
LIBS=
SRC=src/main.c
BIN=2048
all: $(SRC)
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)
install: $(BIN)
	install -d $(BINDIR)
	install $(BIN) $(BINDIR)
clean:
	rm -f $(BIN)
uninstall:
	rm -f $(BINDIR)/$(BIN)
