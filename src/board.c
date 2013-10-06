#include "board.h"


/*
 * Given a Board_t pointer and a file handle, loads seed data from the file and
 * initializes the board struct with that data. To use a board, this or
 * board_copy must be called before use. board_destroy should be called on
 * the board when it is no longer in use.
 *
 * Parameters: Board_t* board, FILE** infile, int toroidal
 * Side-effects: Loads data from the file handel and initializes the with
 *     data. Uses the toroidal flag to determine if the board should
 *     be toroidal or finite. Finite boards have extra memory allocated around
 *     there border so that escaping cells don't interfere with the boundary of
 *     the board. Exits the program if the file cannot be found or is not a
 *     proper format
 */
void board_init(Board_t* board, FILE* infile, int toroidal)
{
    int i = 0;
    int j = 0;
    int x, y;
    int width, height;
    int num_cells;
    int size;
    unsigned char* cells;
    int current_cell = 0;

    // Load the width, height and cell data from the file
    board->toroidal = toroidal;
    if ((fscanf(infile, "%d", &width)) != 1) {
        printf("Bad file format\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    if ((fscanf(infile, "%d", &height)) != 1) {
        printf("Bad file format\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    if ((width > 640 || width < 5) ||
        (height > 480 || height < 5)) {
        printf("Invalid width/height data.\n"
            "Must be 5 < x < 640 and 5 < y < 480\n"
        );
        exit(EXIT_STATUS_BAD_FILE);
    }
    num_cells = width * height;
    cells = malloc(num_cells * sizeof(unsigned char));
    while ((current_cell = fgetc(infile)) != EOF) {
        if (i > num_cells) {
            printf("File contains too many cell values\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        current_cell -= '0';
        if (current_cell == 0 || current_cell == 1) {
            cells[i++] = current_cell;
        }
    }
    if (i < num_cells - 1) {
        printf("File contains too few cell values\n");
        exit(EXIT_STATUS_BAD_FILE);
    }

    // Use the toroidal flag, width, height and cell data to build
    // the board's data
    board->width = width;
    board->height = height;
    board->display_width = width;
    board->display_height = height;

    if (toroidal == 0) {
        board->width += BOARD_BORDER_SIZE * 2;
        board->height += BOARD_BORDER_SIZE * 2;
        board->display_x = BOARD_BORDER_SIZE;
        board->display_y = BOARD_BORDER_SIZE;
        board->display_width += BOARD_BORDER_SIZE;
        board->display_height += BOARD_BORDER_SIZE;
    } else {
        board->display_x = 0;
        board->display_y = 0;
    }
    size = board->width * board->height;
    board->cells = malloc(size * sizeof(unsigned char));
    // for toroidal boards, simply copy each cell
    if (toroidal) {
        for (i = 0; i < num_cells; i++) {
            board->cells[i] = cells[i];
        }
    } else {
        // non-toroidal boards are surrounded on all sides by 0's for
        // BOARD_BORDER_SIZE columns/rows
        i = 0;
        j = 0;
        for (y = 0; y < board->height; y++) {
            for (x = 0; x < board->width; x++) {
                if (x < BOARD_BORDER_SIZE ||
                    x >= board->display_width ||
                    y < BOARD_BORDER_SIZE ||
                    y >= board->display_height) {
                        board->cells[i] = 0;
                } else {
                    board->cells[i] = cells[j];
                    j++;
                }
                i++;
            }
        }
    }
    free(cells);
}

/*
 * Given two Board_t pointers, copy the data in the second into that of the
 * first. This or board_init must be called on a board before it can be used.
 * board_destroy should be called on the board when it is no longer used.
 * The purpose of board_copy is to initialize a board with values from another
 * board, rather than loading seed data from a file. This is necessary for
 * creating a second board as a sort of "buffer" for the next generation of
 * data.
 *
 * Parameters: Board_t* board_a, const Board_t* board_b
 * Side-Effects: Initialize board_a with data from board_b. Allocates memory
 *     for board_a as needed.
 */
void board_copy(Board_t* board_a, const Board_t* board_b)
{
    int i;
    int size;

    board_a->toroidal = board_b->toroidal;
    board_a->width = board_b->width;
    board_a->height = board_b->height;
    size = board_a->width * board_a->height;
    board_a->cells = malloc(size * sizeof(unsigned char));
    for(i = 0; i < size; i++) {
        board_a->cells[i] = board_b->cells[i];
    }
}


/*
 * Given two Board_t pointers, swaps the cell data inside of each.
 *
 * Parameters: Board_t* board_a, Board_t* board_b
 * Side-Effects: swaps the .cells attribute of each board.
 */
void board_swap(Board_t* board_a, Board_t* board_b)
{
    unsigned char* swap_cells;
    swap_cells = board_a->cells;
    board_a->cells = board_b->cells;
    board_b->cells = swap_cells;
}

/*
 * Given a Board_t* and x,y coordinates, returns the value of the cell at that
 * location. If the board is toroidal and x,y is out of bounds, the coordinates
 * will wrap around to the other side of board. Otherwise, out of bounds
 * coordinates will return 0
 *
 * Parameters: const Board_t* board, int x, int y
 * Side-Effects: None
 * Returns: int value of the cell (0 or 1)
 */
int board_get_cell(const Board_t* board, int x, int y)
{
    int index;

    if (board->toroidal) {
        if (x >= board->width) {
            x -= board->width;
        }
        if (x < 0) {
            x += board->width;
        }
        if (y >= board->height) {
            y -= board->height;
        }
        if (y < 0) {
            y += board->height;
        }
    } else if (x >= board->width || x < 0 || y >= board->height || y < 0) {
        return 0;
    }
    index = y * board->width + x;
    return board->cells[index];
}

/*
 * Given a Board_t pointer and x,y coordinates, returns the number
 * of living neighbors for the cell at x,y.
 *
 * Parameters: const Board_t* board, int x, int y
 * Side-Effects: None
 * Returns: int of the total number of neighbors
 */
int board_count_moore_neighbors(const Board_t* board, int x, int y)
{
    return (
        board_get_cell(board, x - 1, y - 1) +
        board_get_cell(board, x, y - 1) +
        board_get_cell(board, x + 1, y - 1) +
        board_get_cell(board, x - 1, y) +
        board_get_cell(board, x + 1, y) +
        board_get_cell(board, x - 1, y + 1) +
        board_get_cell(board, x, y + 1) +
        board_get_cell(board, x + 1, y + 1)
    );
}

/*
 * Given a Board_t pointer, free the memory allocated for that board.
 *
 * Parameters: Board_t* board
 * Side-Effects: frees memory for the given board.
 */
void board_destroy(Board_t* board)
{
    free(board->cells);
}
