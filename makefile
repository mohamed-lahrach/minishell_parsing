CC=gcc
CFLAGS=-g -Wall
LIBS=-lreadline
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=minishell

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(EXEC)

.PHONY: all clean fclean