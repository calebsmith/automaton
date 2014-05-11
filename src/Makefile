CC = gcc
CFLAGS = -Wall -std=gnu99 -D _BSD_SOURCE -O3
#Uncomment for Ubuntu for now LIBS = -lrt -lncurses -lglfw3 -lGL -lm -lXcursor -lX11 -lXrandr -lpthread -lXi -L/usr/local/lib
LIBS = -lrt -lncurses -lglfw -lGL -L/usr/local/lib
NAME = ../gameoflife
SOURCES = *.c

$(NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(NAME)
