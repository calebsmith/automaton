/*
 * The "lens" acts as a mediator between the front-end display and the data
 * from the "board" that it shows. The lens determines these aspects of the
 * display:
 *
 *     1. What portion of the board should be shown (allow user to navigate
 *         around the board like moving a spyglass across a map).
 *     2. If the area of the board that can been is smaller than the overall
 *         display area, put it into the center
 *     3. If the entire board is smaller than the display area, scale the
 *         display of the board appropriately (i.e. zoom in)
 *
 * lens_init() should be called before anything else is used.
 * lens_set() should be called each time at the beginning of the display
 *     rendering function.
 * lens_move() and its shortcut functions are used to move the lens around
 *     on the board
 */
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
    int scale;              // size of each cell in pixels/characters

} Lens_t;

#define LENS_MOVEMENT_RATE 5

// Constants for directions
// TODO: Define these elsewhere for more general use
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


void lens_init(Lens_t* lens, const Board_t* board, int width, int height, bool scale);
void lens_set(Lens_t* lens, const Board_t* board, int width, int height);
void lens_set_offset_bounds(Lens_t* lens, const Board_t* board, int width, int height);
void lens_set_dimensions(Lens_t* lens, const Board_t* board, int width, int height);
void lens_set_display_offset(Lens_t* lens, const Board_t* board, int width, int height);
void lens_move(Lens_t* lens, int direction);
void lens_move_left(Lens_t* lens);
void lens_move_right(Lens_t* lens);
void lens_move_up(Lens_t* lens);
void lens_move_down(Lens_t* lens);
void lens_zoom_in(Lens_t* lens);
void lens_zoom_out(Lens_t* lens);
#endif
