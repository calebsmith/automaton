CC = gcc
CFLAGS = -Wall -std=gnu99 `guile-config compile` -D _BSD_SOURCE -O3
LIBS = -lrt `guile-config link` -lncurses -lglfw3 -lGL -lm -lXcursor -lX11 -lXrandr -lpthread -lXi -L/usr/local/lib
#LIBS = -lrt `guile-config link` -lncurses -lglfw -lGL -L/usr/local/lib
NAME = ../gameoflife
HEADERS = board.h lens.h rule.h world.h backend.h text_ui.h graphical_ui.h scm.h
SOURCES = main.c $(HEADERS:.h=.c)
OBJECTS = $(SOURCES:.c=.o)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

board.o: board.h
lens.o: lens.h board.o
rule.o: rule.h
world.o: world.h board.o rule.o
scm.o: scm.h world.o
backend.o: backend.h scm.o world.o

text_ui.o: text_ui.h backend.o lens.o
graphical_ui.o: graphical_ui.h backend.o lens.o
main.o: $(HEADERS) text_ui.o graphical_ui.o


clean:
	rm -f $(NAME)
	rm -f *.o
