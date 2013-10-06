#include "backend.h"

/*
 * Wait for <sleep_time> adjusted by the difference of the current
 * time and the finishing time of the last loop execution.
 *
 * Note: This implementation is POSIX only.
 *
 * Parameters: unsigned long long int sleep_time, unsigned long long int* last_time
 * Side-Effects: Sets *last_time to the number of nanoseconds on the real-time
 *     clock after the CPU sleep has occured. This is used to calculate the
 *     amount of time to sleep in the next loop execution.
*/
void wait(unsigned long long int sleep_time, unsigned long long int* last_time)
{
    unsigned long long now, delta;
    struct timespec tm, sleep_tm;

    sleep_tm.tv_sec = 0;

    // obtain current time
    clock_gettime(CLOCK_REALTIME, &tm);
    now = tm.tv_nsec + tm.tv_sec * NANO;
    // calculate amount of time difference
    delta = now - *last_time;
    if (*last_time > 0) {
        // wait the configured amount of time, minus the time the loop took
        sleep_tm.tv_nsec = sleep_time - delta;
        nanosleep(&sleep_tm, NULL);
    }
    // obtain current time after waiting and store for next call
    clock_gettime(CLOCK_REALTIME, &tm);
    *last_time = tm.tv_nsec + tm.tv_sec * NANO;
}

/*
 * Determines the next board based on the cells of the current one, then
 * swaps them to move to the next generation. Follows Conway's Game of Life
 * rules: B3/S23
 *
 * Parameters: Board_t* next_board, Board_t* board
 * Return: void
 * Side-Effect: Overwrite next_board with data for the next generation and
 *     swap the boards
 */
void generate(Board_t* next_board, Board_t* board)
{
    int x, y;
    int index;
    int num_neighbors;
    int current_cell;

    // Visit each cell, count its neighbors and determine its living/dead
    // status in the next generation. Then apply to the next_board
    for(y = 0; y < board->height; y++) {
        for(x = 0; x < board->width; x++) {
            num_neighbors = board_count_moore_neighbors(board, x, y);
            index = y * board->width + x;
            current_cell = board_get_cell(board, x, y);
            if (current_cell) {
                if (num_neighbors == 2 || num_neighbors == 3) {
                    // survival
                    next_board->cells[index] = LIVE;
                } else {
                    // overcrowding or underpopulation
                    next_board->cells[index] = DEAD;
                }
            } else {
                if (num_neighbors == 3) {
                    // reproduction
                    next_board->cells[index] = LIVE;
                } else {
                    // remain dead
                    next_board->cells[index] = DEAD;
                }
            }
        }
    }
    // swap boards
    board_swap(board, next_board);
}

