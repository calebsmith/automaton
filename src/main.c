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
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "backend.h"
#include "text_ui.h"
#include "graphical_ui.h"

#define EXIT_STATUS_TOO_FEW_ARGUMENTS 1
#define EXIT_STATUS_NO_FILE 2


const char* USAGE_STRING = ""
"The command format is:\n"
"\n"
"gameoflife [Options] seed_filename\n"
"\n"
"Command line flags include:\n"
"\n"
"* -g, --graphical - Use graphical mode (OpenGL rendering)\n"
"* -f, --fullscreen - Use fullscreen rendering rather than windowed (pair with\n"
"        -g option, otherwise has no effect)\n"
"* -t, --toroidal - Make the board toroidal (wrap around)\n"
"* -s number, --speed=number - Set the amount of microseconds between\n"
"    generations. (Default is 85000)\n"
"\n"
"Examples\n"
"\n"
"    #Fast, full-screen graphical display\n"
"    ./gameoflife -g -f -s 1000 data/gosper_gun.dat\n"
"    #Slow, toroidal textual display\n"
"    ./gameoflife -s 500000 data/r_pentomino.dat\n"
"\n"
"\n"
"Usage During Runtime\n"
"--------------------\n"
"\n"
"During program execution, the following keys have the corresponding effects:\n"
"\n"
"* escape or q - quit\n"
"* p - pause\n"
"* r - resume\n"
"* h / left-arrow - move view left\n"
"* j / down-arrow - move view down\n"
"* k / up-arrow - move view up\n"
"* l / right-arrow - move view right\n";


// Stores command line arguments after parsing
typedef struct {
    bool fullscreen;
    bool help;
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
    if (config.help) {
        printf("%s", USAGE_STRING);
        return 0;
    }

    // Create a game board, and a next board
    Board_t board, next_board;
    FILE *infile;

    if ((infile = fopen(config.filename, "r")) == NULL) {
        printf("Could not open file %s\n", config.filename);
        exit(EXIT_STATUS_NO_FILE);
    } else {
        board_init(&board, infile, config.toroidal);
        fclose(infile);
    }
    board_copy(&next_board, &board);

    if (config.graphical) {
        main_glfw(&board, &next_board, config.sleep_time * 1000, config.fullscreen);
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

    config.fullscreen = false;
    config.help = false;
    config.graphical = 0;
    config.toroidal = 0;
    config.sleep_time = DEFAULT_SLEEP_TIME;

    for(i = 1; i < argc; i++) {
        arg = argv[i];
        if (strncmp(arg, "-t", 2) == 0 ||
            strncmp(arg, "--toroidal", 10) == 0) {
            // handle toroidal flag
            config.toroidal = 1;
        } else if (strncmp(arg, "-g", 2) == 0 ||
            strncmp(arg, "--graphical", 11) == 0) {
            // handle graphical flag
            config.graphical = 1;
        } else if (strncmp(arg, "-f", 2) == 0 ||
            strncmp(arg, "--fullscreen", 12) == 0) {
            // handle fullscreen flag
            config.fullscreen = true;
        } else if (strncmp(arg, "-h", 2) == 0 ||
            strncmp(arg, "--help", 6) == 0) {
            // handle help flag
            config.help = true;
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
    if (config.filename == NULL && config.help == false) {
        printf("Must provide a filename to a data file\n");
        exit(EXIT_STATUS_TOO_FEW_ARGUMENTS);
    }
    return config;
}

