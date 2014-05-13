/*
 */
#ifndef SCM_H
#define SCM_H


#include <libguile.h>

#include "world.h"

void scm_handle_cell(const World_t* world, int cell_x, int cell_y);
void *register_scm_functions(void* data);

#endif
