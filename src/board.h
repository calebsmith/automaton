/*
 * The "board" constitutes a grid of "cells" each with a given state. A board's
 * initial state is loaded from a data file and changes over time
 * deterministically, according to a given algorithm. During the main program
 * loop there are 2 boards, since the algorithm that creates the next state of
 * board does not mutate a given board in doing so, and instead assigns the
 * appropriate next state to an auxillary board. Once the algorithm is ran, the
 * two boards are swapped so that the "next" board is shown, and the "current"
 * board becomes the "next" board in the next pass of the loop.
 *
 * board_init() should be called with a file handle to load the initial state
 * board_copy() should be called to create the auxillary board from the data of
 *     the board created with board_init()
 * board_destroy() *MUST* be called when any board is no longer needed.
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_BORDER_SIZE 30
#define EXIT_STATUS_BAD_FILE 3
#define RULE_FILENAME_LENGTH 200

#define MAX_STATE 255
#define MIN_WIDTH 5
#define MIN_HEIGHT 5
#define MAX_WIDTH 2660
#define MAX_HEIGHT 768 * 2

typedef struct {
    int toroidal;
    // actual width, height in memory
    int width, height;
    // x, y offsets for where to start and stop  displaying board data
    int min_x, min_y;
    int max_x, max_y;
    unsigned char *cells;
    char rule_filename[RULE_FILENAME_LENGTH];
} Board_t;

void board_init(Board_t* board, FILE* infile, int toroidal);
void board_copy(Board_t* board_a, const Board_t* board_b);
void board_swap(Board_t* board_a, Board_t* board_b);
int board_get_cell(const Board_t* board, int x, int y);
int board_count_moore_neighbors(const Board_t* board, int x, int y, unsigned char state);
int board_count_von_neumann_neighbors(const Board_t* board, int x, int y, unsigned char state);
void board_destroy(Board_t* board);
#endif
