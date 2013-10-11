#ifndef TEXT_UI_H
#define TEXT_UI_H

#include <curses.h>

#include "board.h"
#include "lens.h"
#include "backend.h"

WINDOW* init_curses(void);
void display_curses(const Board_t* board, WINDOW* window);
void main_curses(Board_t* board, Board_t* next_board, unsigned long long int sleep_time);

#endif
