#include <stdio.h>
#include <stdlib.h>

#include "board.h"

#define EXIT_STATUS_NO_FILE 1
#define EXIT_STATUS_BAD_FILE 2


void board_init(board_t* board, const char* filename)
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

void board_copy(board_t* board_a, const board_t* board_b)
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


void board_swap(board_t* board_a, board_t* board_b)
{
    int* swap_cells;
    swap_cells = board_a->cells;
    board_a->cells = board_b->cells;
    board_b->cells = swap_cells;
}


int board_get_cell(const board_t* board, int x, int y)
{
    int index;
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
    index = y * board->width + x;
    return board->cells[index];
}

int board_count_neighbors(const board_t* board, int x, int y)
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

void board_destroy(board_t* board)
{
    free(board->cells);
}
