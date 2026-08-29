CC = cc
CFLAGS = -Wall -Wextra -Wpedantic -g -Iinclude

SRC = src/main.c src/todo.c
OBJ = $(SRC:.c=.o)

ctodo: $(OBJ)
	$(CC) $(OBJ) -o ctodo

clean:
	rm -f $(OBJ) ctodo

.PHONY: clean
