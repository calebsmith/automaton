CC = gcc
CFLAGS = -Wall -std=gnu99 `guile-config compile` -D _BSD_SOURCE -D DEBUG -g -O3 `pkg-config --cflags glfw3`
LIBS = -lrt `guile-config link` -lncurses `pkg-config --libs --static glfw3`
NAME = gameoflife
HEADERS = src/board.h src/lens.h src/rule.h src/world.h src/backend.h src/text_ui.h src/graphical_ui.h src/scm.h
SOURCES = src/main.c $(HEADERS:.h=.c)
OBJECTS = $(SOURCES:.c=.o)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

board.o: src/board.h
lens.o: src/lens.h src/board.o
rule.o: src/rule.h
world.o: src/world.h src/board.o src/rule.o
scm.o: src/scm.h src/world.o
backend.o: src/backend.h src/scm.o src/world.o

text_ui.o: src/text_ui.h src/backend.o src/lens.o
graphical_ui.o: src/graphical_ui.h src/backend.o src/lens.o
main.o: $(HEADERS) src/text_ui.o src/graphical_ui.o


clean:
	rm -f $(NAME)
	rm -f src/*.o
	rm gameoflife.tar.gz
	rm -Rf game_of_life/
