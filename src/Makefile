CC = gcc
CFLAGS = -Wall -std=gnu99 -D _BSD_SOURCE
LIBS = -lrt -lncurses
NAME = ../gameoflife
SOURCES = *.c

$(NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(NAME)
