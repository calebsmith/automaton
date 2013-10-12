#include "lens.h"

void lens_init(Lens_t* lens, const Board_t* board)
{
    lens->x_offset = 0;
    lens->y_offset = 0;
    lens->x_display_offset = 0;
    lens->y_display_offset = 0;
}

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

    // TODO: set scale > 1 here depending on the number of times lens will
    // fit in the board


    // Set display offset to match x,y offset, but apply BOARD_BORDER_SIZE for
    // non-toroidal boards
    if (board->toroidal) {
        lens->x_display_offset = lens->x_offset;
        lens->y_display_offset = lens->y_offset;
    } else {
        lens->x_display_offset = lens->x_offset + BOARD_BORDER_SIZE;
        lens->y_display_offset = lens->y_offset + BOARD_BORDER_SIZE;
    }
    // Offset the display so that its centered
    lens->x_display_offset -= (width - diff_lens_x) / 2;
    lens->y_display_offset -= (height - diff_lens_y) / 2;

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
