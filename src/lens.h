#ifndef LENS_H
#define LENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"

typedef struct {
    int min_x, min_y;
    int max_x, max_y;
    int x_offset, y_offset;
    int scale;
} Lens_t;

void lens_set(Lens_t* lens, const Board_t* board, int width, int height);
#endif
