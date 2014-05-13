/*
 * The "world" consists of a "board" that represents the current state of the
 * simulation, along with a second board for storing the next generation.
 * The world also contains a "rule" that represents the rule set that will be
 * followed when a generation is calculated.
 *
 */
#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "rule.h"

#define EXIT_STATUS_TOO_FEW_ARGUMENTS 1
#define EXIT_STATUS_NO_FILE 2
#define MAX_FILENAME 500

#define DEFAULT_SLEEP_TIME 85000

// Stores command line arguments after parsing
typedef struct {
    bool fullscreen;
    bool help;
    int graphical;
    int toroidal;
    unsigned long long int sleep_time;
    char filename[MAX_FILENAME + 1];
} Config_t;

// Stores the boards and ruleset
typedef struct {
    Board_t* board;
    Board_t* next_board;
    Rule_t* rule;
} World_t;


Config_t get_config(int argc, char* argv[]);

int world_init(World_t* world, Config_t config);
void world_destroy(World_t* world);
#endif
