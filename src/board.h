#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>

#define BOARD_BORDER_SIZE 30
#define EXIT_STATUS_BAD_FILE 3

typedef struct {
    int toroidal;
    // actual width, height in memory
    int width, height;
    // x, y offsets for where to start displaying board data
    int display_x, display_y;
    // x, y limits that mark the edge of where cells should be displayed
    int display_width, display_height;
    unsigned char *cells;
} Board_t;

void board_init(Board_t* board, FILE* infile, int toroidal);
void board_copy(Board_t* board_a, const Board_t* board_b);
void board_swap(Board_t* board_a, Board_t* board_b);
int board_get_cell(const Board_t* board, int x, int y);
int board_count_moore_neighbors(const Board_t* board, int x, int y);
void board_destroy(Board_t* board);
#endif
