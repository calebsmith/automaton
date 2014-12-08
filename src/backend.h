/*
 * Defines the basic operations of the "backend" portion of the program. (The
 * back and frontends of the program are one process/one thread, but are
 * separated into concerns regarding data (backend) and display (frontend).
 */
#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>
// clock_gettime is not implemented in OS X. Here is a workaround.
// See backend.c for the implementation
#ifdef __MACH__
#include <sys/time.h>
extern int clock_gettime(int clk_id, struct timespec* t);
#define CLOCK_REALTIME 0
#endif

#include "world.h"
#include "scm.h"

#define NANO 1000000000 // convert nanoseconds to seconds
// Default number of milliseconds to sleep
#define DEFAULT_SLEEP_TIME 85000

void generate(World_t* world);
void handle_transition_rule(World_t* world, int x, int y);
void wait_until(unsigned long long int sleep_time, unsigned long long int* last_time);

#endif
