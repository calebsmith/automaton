#ifndef RULE_H
#define RULE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "board.h"

#define COLOR_STRING_LENGTH 6
#define NEIGHBOR_STRING_LENGTH  20


typedef enum {
    NEIGHBOR_MOORE,
    NEIGHBOR_VON_NEUMANN
} Neighbor_t;


typedef int (*NeighborFunction_t) (const Board_t*, int, int, unsigned char);


typedef struct {
    unsigned char red;
    unsigned char blue;
    unsigned char green;
} Color_t;


typedef struct {
    unsigned char begin;
    unsigned char end;
    unsigned char neighbor_state;
    bool negator;
    int size;
    int* transitions;

} Transition_t;


typedef struct {
    NeighborFunction_t neighbor_func;
    unsigned short int num_states;
    unsigned short int state_chars[MAX_STATE];
    Color_t state_colors[MAX_STATE];
    int transition_length;
    Transition_t** transitions;
} Rule_t;


int rule_init(Rule_t* rule, FILE* infile);
void rule_destroy(Rule_t* rule);

int transitions_init(Transition_t** transition, int transition_length, unsigned short int num_states, FILE* infile);
int _read_transition_line(FILE* file, int* size, int** results_container);

#ifdef DEBUG
void rule_display(Rule_t* rule);
#endif

#endif
