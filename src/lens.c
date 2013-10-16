#include "lens.h"

/*
 * Initializes the values of the lens before it is used. `width` and `height`
 * parameters are the expected values of the display width/height and `scale`
 * determines if the lens should scale the display (zoom in).
 *
 * N.B. Curses display doesn't support zooming, but the graphical display
 * does
 *
 * Parameters: Lens_t* lens, const Board_t* board, int width, int height,
 *     bool scale
 * Side-Effects: Sets inital values for the lens' offsets, min and max values,
 *     and scale (if scale is passed as true)
 */
void lens_init(Lens_t* lens, const Board_t* board, int width, int height, bool scale)
{
    int x_scale, y_scale;
    int diff_lens_x;
    int diff_lens_y;

    lens->x_offset = 0;
    lens->y_offset = 0;
    lens->x_display_offset = 0;
    lens->y_display_offset = 0;
    lens->scale = 1;
    lens_set(lens, board, width, height);

    // set scale > 1 here depending on the number of times lens will fit in the
    // board (only if scale is passed as true)
    if (scale) {
        diff_lens_x = lens->max_x - lens->min_x;
        diff_lens_y = lens->max_y - lens->min_y;
        x_scale = width / diff_lens_x;
        y_scale = height / diff_lens_y;
        lens->scale = (x_scale < y_scale) ? x_scale: y_scale;
    }
    if (lens->scale == 0) {
        lens->scale = 1;
    }
    lens->min_scale = lens->scale;
}

/*
 * Assures x and y offsets are within bounds so the lens doesn't go over the
 * edge of the board.
 *
 * Parameters: Lens_t* lens, const Board_t* board, int width, int height
 * Side-Effects: Sets x_offset and y_offset of the lens to a value in bounds
 */
void lens_set_offset_bounds(Lens_t* lens, const Board_t* board, int width, int height)
{
    int diff_board_x, diff_board_y;
    int diff_lens_x, diff_lens_y;

    // Determine width of board
    diff_board_x = board->max_x - board->min_x;
    diff_board_y = board->max_y - board->min_y;
    // Determine visible size of lens (min of board size and display size)
    diff_lens_x = (diff_board_x < width) ? diff_board_x: width;
    diff_lens_y = (diff_board_y < height) ? diff_board_y: height;

    int board_middle_x = (diff_board_x / 2) + board->min_x;
    int board_middle_y = (diff_board_y / 2) + board->min_y;
    int visible_offset_x = (width / lens->scale) / 2 + 1;
    int visible_offset_y = (height / lens->scale) / 2 + 1;

    int left_edge = board_middle_x - visible_offset_x + lens->x_offset;
    int right_edge = board_middle_x + visible_offset_x + lens->x_offset;
    int top_edge = board_middle_y - visible_offset_y + lens->y_offset;
    int bottom_edge = board_middle_y + visible_offset_y + lens->y_offset;

    if (right_edge >= board->max_x && left_edge <= board->min_x) {
        // limit x offset if width of board fits in view
        if (lens->x_offset + board->min_x + diff_lens_x > board->max_x) {
            lens->x_offset = board->max_x - diff_lens_x - board->min_x;
        }
        if (lens->x_offset < 0) {
            lens->x_offset = 0;
        }
    } else {
        // limit x offset to edges if board does not fit in view
        if (right_edge > board->max_x) {
            lens->x_offset = board->max_x - visible_offset_x - board_middle_x;
        }
        if (left_edge < board->min_x) {
            lens->x_offset = board->min_x + visible_offset_x - board_middle_x;
        }
    }
    if (bottom_edge >= board->max_y && top_edge <= board->min_y) {
        // limit y offset if width of board fits in view
        if (lens->y_offset + board->min_y + diff_lens_y > board->max_y) {
            lens->y_offset = board->max_y - diff_lens_y - board->min_y;
        }
        if (lens->y_offset < 0) {
            lens->y_offset = 0;
        }
    } else {
        // limit y offset to edges if board does not fit in view
        if (bottom_edge > board->max_y) {
            lens->y_offset = board->max_y - visible_offset_y - board_middle_y;
        }
        if (top_edge < board->min_y) {
            lens->y_offset = board->min_y + visible_offset_y - board_middle_y;
        }
    }
}

/*
 * Sets the boundaries of the lens based on the display and board size as
 * well as the current amount of x,y offset.
 *
 * Parameters: Lens_t* lens, const Board_t* board, int width, int height
 * Side-Effects: Sets min and max x,y values of the lens to indicate the
 *     current boundaries of the lens
 */
void lens_set_dimensions(Lens_t* lens, const Board_t* board, int width, int height)
{
    int diff_board_x, diff_board_y;
    int diff_lens_x, diff_lens_y;

    // Determine width of board
    diff_board_x = board->max_x - board->min_x;
    diff_board_y = board->max_y - board->min_y;
    // Determine visible size of lens (min of board size and display size)
    diff_lens_x = (diff_board_x < width) ? diff_board_x: width;
    diff_lens_y = (diff_board_y < height) ? diff_board_y: height;

    lens->min_x = lens->x_offset + board->min_x;
    lens->min_y = lens->y_offset + board->min_y;
    lens->max_x = lens->min_x + diff_lens_x;
    lens->max_y = lens->min_y + diff_lens_y;
}

/*
 * Sets the x,y display_offsets based on the lens max/min values and the scale
 *
 * Parameters: Lens_t* lens, const Board_t* board, int width, int height
 * Side-Effects: sets x_display_offset and y_display_offset of lens
 */
void lens_set_display_offset(Lens_t* lens, const Board_t* board, int width, int height)
{
    int diff_lens_x = lens->max_x - lens->min_x;
    int diff_lens_y = lens->max_y - lens->min_y;
    int scale;

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
    scale = (lens->scale >= 1) ? lens->scale : 1;
    lens->x_display_offset -= ((width - diff_lens_x * scale) / 2) / scale;
    lens->y_display_offset -= ((height - diff_lens_y * scale) / 2) / scale;
}

/*
 * Sets all values of the lens based on the current display parameters and
 * board
 *
 * Parameters: Lens_t* lens, const Board_t* board, int width, int height
 * Side-Effects: Sets all values of the lens based on the display width/height
 *     and board size
 */
void lens_set(Lens_t* lens, const Board_t* board, int width, int height)
{
    lens_set_offset_bounds(lens, board, width, height);
    lens_set_dimensions(lens, board, width, height);
    lens_set_display_offset(lens, board, width, height);
}

/*
 * Changes the lens' x,y offsets based on the direction passed in
 *
 * Parameters: Lens_t* lens, int direction
 */
void lens_move(Lens_t* lens, int direction)
{
    switch(direction) {
        case LEFT:
            lens->x_offset -= LENS_MOVEMENT_RATE;
            break;
        case RIGHT:
            lens->x_offset += LENS_MOVEMENT_RATE;
            break;
        case UP:
            lens->y_offset -= LENS_MOVEMENT_RATE;
            break;
        case DOWN:
            lens->y_offset += LENS_MOVEMENT_RATE;
            break;
    }
}

// Shortcut for lens_move(lens, LEFT)
void lens_move_left(Lens_t* lens)
{
    lens_move(lens, LEFT);
}

// Shortcut for lens_move(lens, RIGHT)
void lens_move_right(Lens_t* lens)
{
    lens_move(lens, RIGHT);
}

// Shortcut for lens_move(lens, UP)
void lens_move_up(Lens_t* lens)
{
    lens_move(lens, UP);
}

// Shortcut for lens_move(lens, DOWN)
void lens_move_down(Lens_t* lens)
{
    lens_move(lens, DOWN);
}

void lens_zoom_in(Lens_t* lens)
{
    if (lens->scale < 20) {
        lens->scale++;
    }
}

void lens_zoom_out(Lens_t* lens)
{
    if (lens->scale > lens->min_scale) {
        lens->scale--;
    }
}
