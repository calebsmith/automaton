/*
A straightforward implementation of Conway's Game of Life in C
Copyright (C) 2013  Caleb Smith

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>

#include "board.h"

const int LIVE = 1; // represents a living cell on the board.
const int DEAD = 0; // represents a dead cell on the board.
const unsigned long long NANO = 1000000000; // convert nanoseconds to seconds

WINDOW* init_curses(void);
void display(const Board_t* board, WINDOW* window);
void generate(Board_t* next_board, Board_t* board);
void wait(const Board_t* board, unsigned long long int* last_time);


int main(int argc, char* argv[])
{
    WINDOW* window; // the curses window
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long* last_time;
    unsigned long long last_time_default = 0;
    last_time = &last_time_default;

    if (argc < 2) {
        printf("Must provide a filename to a data file\n");
        exit(1);
    }
    // Create a game board, and a next board
    Board_t board, next_board;
    board_init(&board, argv[1]);
    board_copy(&next_board, &board);

    window = init_curses();
    // Display game board, find next generation, wait for time and loop
    while(1) {
        display(&board, window);
        generate(&next_board, &board);
        wait(&board, last_time);
    }
    endwin();
    board_destroy(&board);
    board_destroy(&next_board);
    return 0;
}


/*
 * initializes and clears a curses window with raw input mode, and no echo.
 *
 * parameters: void
 * returns: window* referring to the curses window
 */
WINDOW* init_curses(void)
{
    // create and prepare a curses window
    WINDOW* window = initscr();
    cbreak();
    noecho();
    clear();
    return window;
}


/*
 * Displays the current board in a curses window
 *
 * Parameters: Board_t* board, WINDOW* display_area
 * Return: void
 */
void display(const Board_t* board, WINDOW* window)
{
    int x, y;
    int display_width, display_height;
    int value;

    for(y = 0; y < board->height; y++) {
        for(x = 0; x < board->width; x++) {
            // display 0 for each living cell, space for each dead cell
            value = board_get_cell(board, x, y);
            move(y, x);
            delch();
            // determine terminal's rows and columns. Display cell if in bounds
            getmaxyx(window, display_height, display_width);
            if (y < display_height && x < display_width) {
                if (value == LIVE) {
                    insch('0');
                } else {
                    insch(' ');
                }
            }
        }
    }
    refresh();
}


/*
 * Determines the next board based on the cells of the current one, then
 * swaps them to move to the next generation
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
    int x_lower_bound, x_higher_bound;
    int y_lower_bound, y_higher_bound;

    // Determine boundaries of the board based on its toroidal flag
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
    // Visit each cell, count its neighbors and determine its living/dead
    // status in the next generation. Then apply to the next_board
    for(y = y_lower_bound; y < y_higher_bound; y++) {
        for(x = x_lower_bound; x < x_higher_bound; x++) {
            num_neighbors = board_count_neighbors(board, x, y);
            index = y * board->width + x;
            current_cell = board_get_cell(board, x, y);
            if (current_cell) {
                if (num_neighbors < 2) {
                    // underpopulation
                    next_board->cells[index] = DEAD;
                } else if (num_neighbors == 2 || num_neighbors == 3) {
                    // healthy living
                    next_board->cells[index] = LIVE;
                } else if (num_neighbors > 3) {
                    // overcrowding
                    next_board->cells[index] = DEAD;
                }
            } else {
                if (num_neighbors == 3) {
                    // reproduction
                    next_board->cells[index] = LIVE;
                } else {
                    // stasis
                    next_board->cells[index] = current_cell;
                }
            }
        }
    }
    // swap boards
    board_swap(board, next_board);
}


/*
 * Wait for board's <sleep_time> adjusted by the difference of the current time
 * and the finishing time of the last loop execution.
 *
 * Note: This implementation is POSIX only.
 *
 * Parameters: const Board_t* board, unsigned long long int* last_time
 * Side-Effects: Sets *last_time to the number of nanoseconds on the real-time
 *     clock after the CPU sleep has occured. This is used to calculate the
 *     amount of time to sleep in the next loop execution.
*/
void wait(const Board_t* board, unsigned long long int* last_time)
{
    unsigned long long now;
    struct timespec tm, sleep_tm;

    sleep_tm.tv_sec = 0;

    // obtain current time
    clock_gettime(CLOCK_REALTIME, &tm);
    now = tm.tv_nsec + tm.tv_sec * NANO;
    if (last_time > 0) {
        // calculate amount of time to wait
        sleep_tm.tv_nsec = *last_time + board->sleep_time * 1000 - now;
        nanosleep(&sleep_tm, NULL);
    }
    // obtain current time after waiting and store for next call
    clock_gettime(CLOCK_REALTIME, &tm);
    *last_time = tm.tv_nsec + tm.tv_sec * NANO;
}
