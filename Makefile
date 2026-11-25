CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Iinclude
SRC = $(wildcard src/*.c)

biblioteca: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o biblioteca

run: biblioteca
	./biblioteca

clean:
	rm -f biblioteca biblioteca.exe

.PHONY: run clean