CC = gcc
CFLAGS = -Wall -std=c99
NAME = life
SOURCES = *.c

$(NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(NAME)
