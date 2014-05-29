/*
 */
#ifndef SCM_H
#define SCM_H


#include <libguile.h>

#include "world.h"

void scm_generate(World_t* world);
SCM scm_get_cell(SCM x, SCM y);
SCM scm_von_neumann_neighbors(SCM x_in, SCM y_in, SCM state_in);
SCM scm_moore_neighbors(SCM x_in, SCM y_in, SCM state_in);
void *register_scm_functions(void* data);

#endif
