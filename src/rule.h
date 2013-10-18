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


typedef struct {
    unsigned char red;
    unsigned char blue;
    unsigned char green;
} Color_t;


typedef struct {
    unsigned short int num_states;
    Neighbor_t neighbor_type;
    unsigned short int state_chars[MAX_STATE];
    Color_t state_colors[MAX_STATE];
    int num_transitions;
    unsigned char* transition_begin;
    unsigned char* transition_end;
    int* transition_neighbor_state;
    int* transition_sizes;
    int** transitions;
} Rule_t;


int rule_init(Rule_t* rule, FILE* infile);
void rule_destroy(Rule_t* rule);
int _read_transition_line(FILE* file, int* size, int index,
    int** results_container
);

#ifdef DEBUG
void rule_display(Rule_t* rule);
#endif

#endif
