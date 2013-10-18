#include "backend.h"

NeighborFunction_t NEIGHBOR_FUNC_LOOKUP[] = {
    [NEIGHBOR_MOORE]=&board_count_moore_neighbors,
    [NEIGHBOR_VON_NEUMANN]=&board_count_von_neumann_neighbors
};

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
 * Parameters: Board_t* next_board, Board_t* board
 * Return: void
 * Side-Effect: Overwrite next_board with data for the next generation and
 *     swap the boards
 */
void generate(Board_t* next_board, Board_t* board, Rule_t* rule)
{
    int x, y;
    NeighborFunction_t neighbor_count_func;

    // Determine the neighbor counting function to be used based on the
    // neighbor type set in the rule (e.g. Moore neighbors)
    neighbor_count_func = NEIGHBOR_FUNC_LOOKUP[rule->neighbor_type];

    // Visit each cell, count its neighbors and determine its state in the next
    // generation. Then swap to the next_board
    for (y = 0; y < board->height; y++) {
        for (x = 0; x < board->width; x++) {
            handle_transition_rule(
                board, next_board, rule, neighbor_count_func, x, y
            );
        }
    }
    // swap boards
    board_swap(board, next_board);
}

/*
 * Determines the next state of a cell at x, y in the given `board` and applies
 * it to `next_board` according to the given `rule`.
 */
void handle_transition_rule(
    Board_t* board, Board_t* next_board, Rule_t* rule, NeighborFunction_t neighbor_count_func,
    int x, int y)
{
    int num_neighbors;
    int transition_size;
    int index;
    unsigned char current_cell;
    int i, j;
    bool changed = false;

    index = y * board->width + x;
    current_cell = board_get_cell(board, x, y);
    // Go through each transition rule
    for (i = 0; i < rule->num_transitions; i++) {
        if (current_cell == rule->transition_begin[i]) {
            transition_size = rule->transition_sizes[i];
            // If the rule involves neighbor counting
            if (transition_size > 0) {
                num_neighbors = neighbor_count_func(
                    board, x, y, rule->transition_neighbor_state[i]
                );
                for (j = 0; j < transition_size; j++) {
                    // Apply transition if count of neighbors matches
                    // any in the list
                    if (!rule->transition_negator[i]) {
                        if (num_neighbors == rule->transitions[i][j]) {
                            next_board->cells[index] = rule->transition_end[i];
                            changed = true;
                            break;
                        }
                    } else {
                        // Make the transition *unless* the right
                        // number of neighbors are present
                        changed = true;
                        next_board->cells[index] = rule->transition_end[i];
                        if (num_neighbors == rule->transitions[i][j]) {
                            next_board->cells[index] = current_cell;
                            break;
                        }
                    }
                }
            } else {
                // Rule should occur regardless of neighbors
                next_board->cells[index] = rule->transition_end[i];
                changed = true;
            }
        }
    }
    // The cell is unchanged so far, just copy the old value
    if (!changed) {
        next_board->cells[index] = current_cell;
    }
}
