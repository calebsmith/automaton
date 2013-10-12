#include "lens.h"

void lens_set(Lens_t* lens, const Board_t* board, int width, int height)
{
    int diff_board_x, diff_board_y;
    int diff_lens_x, diff_lens_y;

    lens->x_offset = 0;
    lens->y_offset = 0;

    diff_board_x = board->max_x - board->min_x;
    diff_board_y = board->max_y - board->min_y;

    diff_lens_x = (diff_board_x < width) ? diff_board_x: width;
    diff_lens_y = (diff_board_y < height) ? diff_board_y: height;

    lens->min_x = lens->x_offset + board->min_x;
    lens->min_y = lens->y_offset + board->min_y;
    lens->max_x = lens->min_x + diff_lens_x;
    lens->max_y = lens->min_y + diff_lens_y;
    lens->scale = 1;

    // TODO: Auto center the lens. For now, just set at top left
    //
    // TODO: set scale > 1 here depending on the number of times lens will
    // fit in the board
}
