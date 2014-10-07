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
 *     their border so that escaping cells don't interfere with the boundary of
 *     the board. Exits the program if the file cannot be found or is not a
 *     proper format
 */
void board_init(Board_t* board, FILE* infile, int toroidal)
{
    int x = 0, y = 0;
    int width, height;
    int cell_state = 0;
    char rule_filename_path[RULE_FILENAME_LENGTH] = "rules/";

    // Load the width, height
    board->toroidal = toroidal;
    if ((fscanf(infile, "%200s", board->rule_filename)) != 1) {
        printf("Bad file format\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    strncat(rule_filename_path, board->rule_filename, RULE_FILENAME_LENGTH - 6);
    strncat(rule_filename_path, ".rule", 5);
    strncpy(board->rule_filename, rule_filename_path, RULE_FILENAME_LENGTH);
    if ((fscanf(infile, "%d,%d", &width, &height)) != 2) {
        printf("Bad file format\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    /*
     * TODO: change limits to something more reasonable, and document
     */
    if ((width > MAX_WIDTH || width < MIN_WIDTH) ||
        (height > MAX_HEIGHT || height < MIN_HEIGHT)) {
        printf("Invalid width/height data.\n"
            "Must be %d < x < %d and %d < y < %d\n",
            MIN_WIDTH, MAX_WIDTH, MIN_HEIGHT, MAX_HEIGHT
        );
        exit(EXIT_STATUS_BAD_FILE);
    }
    // Use the toroidal flag, to determine actual width, height for the board
    board->width = width;
    board->height = height;
    if (toroidal) {
        board->min_x = 0;
        board->min_y = 0;
        board->max_x = width;
        board->max_y = height;
    } else {
        // Put an empty border of BOARD_BORDER_SIZE around non-toroidal boards
        board->width += BOARD_BORDER_SIZE * 2;
        board->height += BOARD_BORDER_SIZE * 2;
        board->min_x = BOARD_BORDER_SIZE;
        board->min_y = BOARD_BORDER_SIZE;
        board->max_x = board->min_x + width;
        board->max_y = board->min_y + height;
    }
    board->cells = (unsigned char*) calloc(
        board->width * board->height, sizeof(unsigned char)
    );
    // Load cell states in the format state:x,y from the file
    while (fscanf(infile, "%d:%d,%d", &cell_state, &x, &y) == 3) {
        if (cell_state >= 0 && cell_state <= MAX_STATE) {
            if (x > width || y > height || x < 0 || y < 0) {
                printf("Invalid x,y value: %d,%d is out of bounds\n", x, y);
                free(board->cells);
                exit(EXIT_STATUS_BAD_FILE);
            }
            x += board->min_x;
            y += board->min_y;
            board->cells[y * board->width + x] = cell_state;
        }
    }
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


int board_get_toroidal(const Board_t* board)
{
    return board->toroidal;
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
unsigned char board_get_cell(const Board_t* board, int x, int y)
{
    int index;

    if (board->toroidal) {
        x %= board->width;
        y %= board->height;
    } else if (x >= board->width || x < 0 || y >= board->height || y < 0) {
        return 0;
    }
    index = y * board->width + x;
    return board->cells[index];
}

void board_set_cell(Board_t* board, int x, int y, unsigned char state)
{
    int index = y * board->width + x;
    int size = board->width * board->height;

    if (index >= 0 && index < size) {
        board->cells[index] = state;
    }
}
/*
 * Given a Board_t pointer and x,y coordinates, returns the number
 * of Moore neighbors for the cell at x,y (diagonally and orthoganally
 * adjacent).
 *
 * Parameters: const Board_t* board, int x, int y
 * Side-Effects: None
 * Returns: int of the total number of neighbors
 */
int board_count_moore_neighbors(const Board_t* board, int x, int y, unsigned char state)
{
    int result = 0;
    if (board_get_cell(board, x - 1, y - 1) == state) result++;
    if (board_get_cell(board, x, y - 1) == state) result++;
    if (board_get_cell(board, x + 1, y - 1) == state) result++;
    if (board_get_cell(board, x - 1, y) == state) result++;
    if (board_get_cell(board, x + 1, y) == state) result++;
    if (board_get_cell(board, x - 1, y + 1) == state) result++;
    if (board_get_cell(board, x, y + 1) == state) result++;
    if (board_get_cell(board, x + 1, y + 1) == state) result++;
    return result;
}

/*
 * Given a Board_t pointer and x,y coordinates, returns the number
 * of Von Neumann neighbors for the cell at x,y (orthoganally adjacent).
 *
 * Parameters: const Board_t* board, int x, int y
 * Side-Effects: None
 * Returns: int of the total number of neighbors
 */
int board_count_von_neumann_neighbors(const Board_t* board, int x, int y, unsigned char state)
{
    int result = 0;
    if (board_get_cell(board, x, y - 1) == state) result++;
    if (board_get_cell(board, x - 1, y) == state) result++;
    if (board_get_cell(board, x + 1, y) == state) result++;
    if (board_get_cell(board, x, y + 1) == state) result++;
    return result;
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
