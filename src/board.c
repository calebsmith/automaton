#include <stdio.h>
#include <stdlib.h>

#include "board.h"

#define EXIT_STATUS_NO_FILE 1
#define EXIT_STATUS_BAD_FILE 2


/*
 * Given a Board_t pointer and a filename, loads seed data from the file and
 * initializes the board struct with that data. To use a board, this or
 * board_copy must be called before use. board_destroy should be called on
 * the board when it is no longer in use.
 *
 * Parameters: Board_t* board, const char* filename
 * Side-effects: Opens the <filename> file, and if successful initializes the
 *     board with data. Exits the program if the file cannot be found or is not
 *     a proper format
 */
void board_init(Board_t* board, const char* filename)
{
    int i = 0;
    int current_cell;
    int size;
    FILE *infile;

    if ((infile = fopen(filename, "r")) == NULL) {
        printf("Could not open file %s\n", filename);
        exit(EXIT_STATUS_NO_FILE);
    } else {
        if((fscanf(infile, "%d", &board->toroidal)) != 1) {
            printf("Bad file format\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        if((fscanf(infile, "%lld", &board->sleep_time)) != 1) {
            printf("Bad file format\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        if((fscanf(infile, "%d", &board->width)) != 1) {
            printf("Bad file format\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        if((fscanf(infile, "%d", &board->height)) != 1) {
            printf("Bad file format\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        if ((board->width > 1024 || board->width < 5) ||
            (board->height > 768 || board->height < 5)) {
            printf("Invalid width/height data.\n"
                "Must be 5 < x < 1024 and 5 < y < 768\n"
            );
            exit(EXIT_STATUS_BAD_FILE);
        }
        size = board->width * board->height;
        board->cells = (int *) malloc(size * sizeof(int));
        while((current_cell = fgetc(infile)) != EOF) {
            if (i > size) {
                printf("File contains too many values\n");
                exit(EXIT_STATUS_BAD_FILE);
            }
            current_cell -= '0';
            if (current_cell == 0 || current_cell == 1) {
                board->cells[i++] = current_cell;
            }
        }
    }
    fclose(infile);
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
    board_a->sleep_time = board_b->sleep_time;
    board_a->width = board_b->width;
    board_a->height = board_b->height;
    size = board_a->width * board_a->height;
    board_a->cells = (int *) malloc(size * sizeof(int));
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
    int* swap_cells;
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
