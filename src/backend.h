/*
 * Defines the basic operations of the "backend" portion of the program. (The
 * back and frontends of the program are one process/one thread, but are
 * separated into concerns regarding data (backend) and display (frontend).
 */
#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>

#include "board.h"
#include "rule.h"

#define NANO 1000000000 // convert nanoseconds to seconds
// Default number of milliseconds to sleep
#define DEFAULT_SLEEP_TIME 85000

void generate(Board_t* next_board, Board_t* board, Rule_t* rule);
void wait(unsigned long long int sleep_time, unsigned long long int* last_time);

#endif
