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
    if (sleep_time > 0 && *last_time > 0) {
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
 * swaps them to move to the next generation.
 *
 * Parameters: World_t* world
 * Return: void
 * Side-Effect: Overwrite next_board with data for the next generation and
 *     swap the boards
 */
void generate(World_t* world)
{
    int x, y;
    // Visit each cell, count its neighbors and determine its state in the next
    // generation. Then swap to the next_board
    for (y = 0; y < world->board->height; y++) {
        for (x = 0; x < world->board->width; x++) {
            handle_transition_rule(world, x, y);
        }
    }
    // swap boards
    board_swap(world->board, world->next_board);
}

/*
 * Determines the next state of a cell at x, y in the board of the given
 * `world` and applies it to next_board according to the world's rule.
 */
void handle_transition_rule(World_t* world, int x, int y)
{
    int num_neighbors;
    Transition_t* transition;
    int index;
    unsigned char current_cell;
    int i, j;
    bool changed = false;

    index = y * world->board->width + x;
    current_cell = board_get_cell(world->board, x, y);
    // Go through each transition rule
    for (i = 0; i < world->rule->transition_length; i++) {
        transition = world->rule->transitions[i];
        if (current_cell == transition->begin) {
            // If the rule involves neighbor counting
            if (transition->size > 0) {
                num_neighbors = world->rule->neighbor_func(
                    world->board, x, y, transition->neighbor_state
                );
                for (j = 0; j < transition->size; j++) {
                    // Apply transition if count of neighbors matches
                    // any in the list
                    if (!transition->negator) {
                        if (num_neighbors == transition->transitions[j]) {
                            world->next_board->cells[index] = transition->end;
                            changed = true;
                            break;
                        }
                    } else {
                        // Make the transition *unless* the right
                        // number of neighbors are present
                        changed = true;
                        world->next_board->cells[index] = transition->end;
                        if (num_neighbors == transition->transitions[j]) {
                            world->next_board->cells[index] = current_cell;
                            break;
                        }
                    }
                }
            } else {
                // Rule should occur regardless of neighbors
                world->next_board->cells[index] = transition->end;
                changed = true;
            }
        }
    }
    // The cell is unchanged so far, just copy the old value
    if (!changed) {
        world->next_board->cells[index] = current_cell;
    }
}
