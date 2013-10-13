/*
 * The text_ui creates a console based front-end display using the curses
 * library. This handles the display of board, as well as listening for
 * keyboard events for program control flow/termination
 */
#ifndef TEXT_UI_H
#define TEXT_UI_H

#include <curses.h>

#include "board.h"
#include "lens.h"
#include "backend.h"

WINDOW* init_curses(void);
void display_curses(const Board_t* board, Lens_t* lens, WINDOW* window);
void main_curses(Board_t* board, Board_t* next_board, unsigned long long int sleep_time);

#endif
