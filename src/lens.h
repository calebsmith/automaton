#ifndef LENS_H
#define LENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"

typedef struct {
    int min_x, min_y;       // starting position in top-left
    int max_x, max_y;       // ending position in bottom-right
    int x_offset, y_offset; // amount of offset of lens from top-left of board
    // Amount to displace the display coordinates x,y from the x,y of board
    int x_display_offset, y_display_offset;
    int scale;

} Lens_t;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


void lens_init(Lens_t* lens, const Board_t* board);
void lens_set(Lens_t* lens, const Board_t* board, int width, int height);
void lens_move(Lens_t* lens, int direction);
void lens_move_left(Lens_t* lens);
void lens_move_right(Lens_t* lens);
void lens_move_up(Lens_t* lens);
void lens_move_down(Lens_t* lens);
#endif
