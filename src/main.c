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

#include "board.h"
#include "algo.h"
#include "text_ui.h"
#include "graphical_ui.h"


// Stores command line arguments after parsing
typedef struct {
    int graphical;
    int toroidal;
    unsigned long long int sleep_time;
    const char* filename;
} Config_t;

Config_t get_config(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    // Parse the command line arguments and store into config
    Config_t config = get_config(argc, argv);

    // Create a game board, and a next board
    Board_t board, next_board;
    board_init(&board, config.filename, config.toroidal);
    board_copy(&next_board, &board);

    if (config.graphical) {
        main_glfw(&board, &next_board, config.sleep_time * 1000);
    } else {
        main_curses(&board, &next_board, config.sleep_time * 1000);
    }
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

    config.graphical = 0;
    config.toroidal = 0;
    config.sleep_time = DEFAULT_SLEEP_TIME;

    for(i = 1; i < argc; i++) {
        arg = argv[i];
        if (strcmp(arg, "-t") == 0 || strcmp(arg, "--toroidal") == 0) {
            // handle toroidal flag
            config.toroidal = 1;
        } else if (strncmp(arg, "-g", 2) == 0 ||
            strncmp(arg, "--graphical", 11) == 0) {
            config.graphical = 1;
        } else if (strcmp(arg, "-s") == 0) {
            if (i + 1 < argc) {
                // handle sleep time argument
                config.sleep_time  = atoi(argv[i + 1]);
                i++;
            } else {
                printf("-s option takes an argument\n");
                exit(1);
            }
        } else if (strncmp(arg, "--sleep=", 8) == 0) {
            arg = strtok(arg, "="); arg = strtok(NULL, "=");
            config.sleep_time = atoi(arg);
        } else {
            // handle filename argument
            config.filename = arg;
        }
    }
    // error handling
    if (config.sleep_time == 0) {
        config.sleep_time = DEFAULT_SLEEP_TIME;
    }
    if (config.filename == NULL) {
        printf("Must provide a filename to a data file\n");
        exit(1);
    }
    return config;
}

