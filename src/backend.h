#ifndef ALGO_H
#define ALGO_H

#include "board.h"

#define NANO 1000000000 // convert nanoseconds to seconds
// Default number of milliseconds to sleep
#define DEFAULT_SLEEP_TIME 85000
#define LIVE 1 // represents a living cell on the board.
#define DEAD 0 // represents a dead cell on the board.

void generate(Board_t* next_board, Board_t* board);
void wait(unsigned long long int sleep_time, unsigned long long int* last_time);

#endif
