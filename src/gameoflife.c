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
#include <string.h>
#include <time.h>
#include <curses.h>

#include "board.h"

const int LIVE = 1; // represents a living cell on the board.
const int DEAD = 0; // represents a dead cell on the board.
const unsigned long long NANO = 1000000000; // convert nanoseconds to seconds
// Default number of milliseconds to sleep
const unsigned long long DEFAULT_SLEEP_TIME = 85000;

// Stores command line arguments after parsing
typedef struct {
    int toroidal;
    unsigned long long int sleep_time;
    const char* filename;
} Config_t;

Config_t get_config(int argc, char* argv[]);
WINDOW* init_curses(void);
void display(const Board_t* board, WINDOW* window);
void generate(Board_t* next_board, Board_t* board);
void wait(const Config_t* config, unsigned long long int* last_time);


int main(int argc, char* argv[])
{
    WINDOW* window; // the curses window
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long last_time = 0;

    // Parse the command line arguments and store into config
    Config_t config = get_config(argc, argv);
    // Create a game board, and a next board
    Board_t board, next_board;
    board_init(&board, config.filename);
    board.toroidal = config.toroidal;
    board_copy(&next_board, &board);

    window = init_curses();
    // Display game board, find next generation, wait for time and loop
    while(1) {
        display(&board, window);
        generate(&next_board, &board);
        wait(&config, &last_time);
    }
    endwin();
    board_destroy(&board);
    board_destroy(&next_board);
    return 0;
}


/*
 * Parses command line arguments and returns them in a Config_t.
 *
 * Parameters: int argc, char* argv[]
 * Returns: Config_t
 * Side-Effects: Exits with status 1 if filename is not provided
 */
Config_t get_config(int argc, char* argv[])
{
    Config_t config;
    int i;
    char *arg;

    if (argc < 2) {
        printf("Must provide a filename to a data file\n");
        exit(1);
    }
    config.toroidal = 0;
    config.sleep_time = DEFAULT_SLEEP_TIME;

    for(i = 0; i < argc; i++) {
        arg = argv[i];
        if (i == argc - 1) {
            // handle filename argument
            config.filename = arg;
        } else {
            // handle toroidal flag
           if (strcmp(arg, "-t") == 0 || strcmp(arg, "--toroidal") == 0) {
               config.toroidal = 1;
           }
           // handle sleep time argument
           if (strcmp(arg, "-s") == 0) {
               config.sleep_time  = atoi(argv[i + 1]);
           } else if (strncmp(arg, "--sleep=", 8) == 0) {
               arg = strtok(arg, "="); arg = strtok(NULL, "=");
               config.sleep_time = atoi(arg);
           }
        }
    }
    if (config.sleep_time == 0) {
        config.sleep_time = DEFAULT_SLEEP_TIME;
    }
    return config;
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
    curs_set(0);
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

    // clear the display of the old frame
    clear();
    for(y = 0; y < board->height; y++) {
        for(x = 0; x < board->width; x++) {
            // display 0 for each living cell, space for each dead cell
            value = board_get_cell(board, x, y);
            move(y, x);
            // determine terminal's rows and columns. Display cell if in bounds
            getmaxyx(window, display_height, display_width);
            if (y < display_height && x < display_width) {
                if (value == LIVE) {
                    insch('o');
                }
            }
        }
    }
    refresh();
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


/*
 * Wait for config's <sleep_time> adjusted by the difference of the current
 * time and the finishing time of the last loop execution.
 *
 * Note: This implementation is POSIX only.
 *
 * Parameters: const Config_t* config, unsigned long long int* last_time
 * Side-Effects: Sets *last_time to the number of nanoseconds on the real-time
 *     clock after the CPU sleep has occured. This is used to calculate the
 *     amount of time to sleep in the next loop execution.
*/
void wait(const Config_t* config, unsigned long long int* last_time)
{
    unsigned long long now, delta, config_sleep_time;
    struct timespec tm, sleep_tm;

    sleep_tm.tv_sec = 0;
    config_sleep_time = config->sleep_time * 1000;

    // obtain current time
    clock_gettime(CLOCK_REALTIME, &tm);
    now = tm.tv_nsec + tm.tv_sec * NANO;
    // calculate amount of time difference
    delta = now - *last_time;
    if (*last_time > 0) {
        // wait the configured amount of time, minus the time the loop took
        sleep_tm.tv_nsec = config_sleep_time - delta;
        nanosleep(&sleep_tm, NULL);
    }
    // obtain current time after waiting and store for next call
    clock_gettime(CLOCK_REALTIME, &tm);
    *last_time = tm.tv_nsec + tm.tv_sec * NANO;
}
