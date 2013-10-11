#ifndef LENS_H
#define LENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"

typedef struct {
    int x, y;
    int width, height;
    int scale;
} Lens_t;

void lens_set(Lens_t* lens, const Board_t* board, int width, int height, bool scale);
#endif
