#include "lens.h"

void lens_set(Lens_t* lens, const Board_t* board, int width, int height, bool scale)
{
    int diff_board_x, diff_board_y;

    diff_board_x = board->max_x - board->min_x;
    diff_board_y = board->max_y - board->min_y;

    // TODO: Auto center the lens. For now, just set at top left
    lens->x = board->min_x;
    lens->y = board->min_y;
    if (width < diff_board_x || height < diff_board_y || !scale) {
        lens->width = width;
        lens->height = height;
        lens->scale = 1;
    } else {
        lens->width = diff_board_x;
        lens->height = diff_board_y;
        // TODO: set scale > 1 here depending on the number of times lens will
        // fit in the board
    }
}
