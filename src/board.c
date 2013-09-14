#include <stdio.h>
#include <stdlib.h>

#include "board.h"

void board_init(board_t* board, const char* filename)
{
    int i = 0;
    int current_cell;
    FILE *infile;

    if ((infile = fopen(filename, "r")) == NULL) {
        printf("Could not open file %s\n", filename);
        exit(1);
    } else {
        if((fscanf(infile, "%d", &board->toroidal)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%lld", &board->sleep_time)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%d", &board->width)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%d", &board->height)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        board->size = board->width * board->height;
        board->cells = (int *) malloc(board->size * sizeof(int));
        while((current_cell = fgetc(infile)) != EOF) {
            if (i > board->size) {
                printf("File contains too many values\n");
                exit(2);
            }
            current_cell -= '0';
            if (current_cell == 0 || current_cell == 1) {
                board->cells[i++] = current_cell;
            }
        }
    }
    fclose(infile);
}

void board_copy(board_t* board_a, board_t* board_b)
{
    int i;

    board_a->toroidal = board_b->toroidal;
    board_a->sleep_time = board_b->sleep_time;
    board_a->width = board_b->width;
    board_a->height = board_b->height;
    board_a->size = board_b->size;
    board_a->cells = (int *) malloc(board_a->size * sizeof(int));
    for(i = 0; i< board_b->size; i++) {
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


int board_get_cell(board_t* board, int x, int y)
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

int board_count_neighbors(board_t* board, int x, int y)
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
