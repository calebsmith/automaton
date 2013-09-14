#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "board.h"

void generate(board_t* next_board, board_t* board);
void display(board_t* board);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Must provide a filename to a data file\n");
        exit(1);
    }
    const char* filename = argv[1];
    // Create a game board, and a next board
    board_t board, next_board;
    board_init(&board, filename);
    board_copy(&next_board, &board);

    // Display game board, find next generation and loop
    while(1) {
        display(&board);
        generate(&next_board, &board);
        usleep(board.sleep_time);
    }
    board_destroy(&board);
    board_destroy(&next_board);
    return 0;
}

void display(board_t* board)
{
    int x, y;
    int value;

    for(y = 0; y < board->height; y++) {
        for(x = 0; x < board->width; x++) {
            value = board_get_cell(board, x, y);
            if (value) {
                printf("O");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}


void generate(board_t* next_board, board_t* board)
{
    int x, y;
    int index;
    int num_neighbors;
    int current_cell;
    int x_lower_bound, x_higher_bound;
    int y_lower_bound, y_higher_bound;

    if (board->toroidal) {
        x_lower_bound = 0;
        y_lower_bound = 0;
        x_higher_bound = board->width;
        y_higher_bound = board->height;
    } else {
        x_lower_bound = 1;
        y_lower_bound = 1;
        x_higher_bound = board->width - 1;
        y_higher_bound = board->height - 1;
    }
    for(y = y_lower_bound; y < y_higher_bound; y++) {
        for(x = x_lower_bound; x < x_higher_bound; x++) {
            num_neighbors = board_count_neighbors(board, x, y);
            index = y * board->width + x;
            current_cell = board_get_cell(board, x, y);
            if (current_cell) {
                if (num_neighbors < 2) {
                    // underpopulation
                    next_board->cells[index] = 0;
                } else if (num_neighbors == 2 || num_neighbors == 3) {
                    // healthy living
                    next_board->cells[index] = 1;
                } else if (num_neighbors > 3) {
                    // overcrowding
                    next_board->cells[index] = 0;
                }
            } else {
                if (num_neighbors == 3) {
                    // reproduction
                    next_board->cells[index] = 1;
                } else {
                    // stasis
                    next_board->cells[index] = current_cell;
                }
            }
        }
    }
    board_swap(board, next_board);
}
