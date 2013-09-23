CC = gcc
CFLAGS = -Wall -std=gnu99 -D _BSD_SOURCE -O3
LIBS = -lrt -lncurses -lglfw -lGL
NAME = ../gameoflife
SOURCES = *.c

$(NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(NAME)
