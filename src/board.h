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
    // x, y offsets for where to start and stop  displaying board data
    int min_x, min_y;
    int max_x, max_y;
    unsigned char *cells;
} Board_t;

void board_init(Board_t* board, FILE* infile, int toroidal);
void board_copy(Board_t* board_a, const Board_t* board_b);
void board_swap(Board_t* board_a, Board_t* board_b);
int board_get_cell(const Board_t* board, int x, int y);
int board_count_moore_neighbors(const Board_t* board, int x, int y);
void board_destroy(Board_t* board);
#endif
