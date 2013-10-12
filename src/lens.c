#include "lens.h"

void lens_set(Lens_t* lens, const Board_t* board, int width, int height)
{
    int diff_board_x, diff_board_y;
    int diff_lens_x, diff_lens_y;

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

void lens_move(Lens_t* lens, const Board_t* board, int direction)
{
    switch(direction) {
        case LEFT:
            if (lens->min_x > board->min_x) {
                lens->x_offset--;
            }
            break;
        case RIGHT:
            if (lens->max_x < board->max_x) {
                lens->x_offset++;
            }
            break;
        case UP:
            if (lens->min_y > board->min_y) {
                lens->y_offset--;
            }
            break;
        case DOWN:
            if (lens->max_y < board->max_y) {
                lens->y_offset++;
            }
            break;
    }
}

void lens_move_left(Lens_t* lens, const Board_t* board)
{
    lens_move(lens, board, LEFT);
}

void lens_move_right(Lens_t* lens, const Board_t* board)
{
    lens_move(lens, board, RIGHT);
}

void lens_move_up(Lens_t* lens, const Board_t* board)
{
    lens_move(lens, board, UP);
}

void lens_move_down(Lens_t* lens, const Board_t* board)
{
    lens_move(lens, board, DOWN);
}
