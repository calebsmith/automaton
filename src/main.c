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

#include "world.h"
#include "scm.h"
#include "text_ui.h"
#include "graphical_ui.h"


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
"* l / right-arrow - move view right\n"
"* i - zoom in\n"
"* o - zoom out\n";


World_t world;

void inner_main(void *nop, int argc, char** argv);


int main(int argc, char* argv[])
{
    scm_boot_guile(argc, argv, &inner_main, NULL);
    return 0;
}


void inner_main(void *nop, int argc, char** argv)
{
    Config_t config;

    // Parse the command line arguments and store into config
    config = get_config(argc, argv);
    if (config.help) {
        printf("%s", USAGE_STRING);
        return;
    }
    // Load board and ruleset into 'world'
    if (world_init(&world, config)) {
        return;
    }
    // Register functions for scheme and load the scheme module
    scm_c_primitive_load("scm/core.scm");
    scm_c_primitive_load("scm/gameoflife.scm");
    scm_with_guile(&register_scm_functions, NULL);
    // Enter main loop
    if (config.graphical) {
        main_glfw(&world, config.sleep_time * 1000, config.fullscreen);
    } else {
        main_curses(&world, config.sleep_time * 1000);
    }
    world_destroy(&world);
}
