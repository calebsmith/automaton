/*
 */
#ifndef SCM_H
#define SCM_H


#include <libguile.h>

#include "world.h"

void scm_generate(World_t* world);
SCM scm_get_cell(SCM x, SCM y);
SCM scm_set_cell(SCM x, SCM y, SCM state);
SCM scm_neighbor_count(SCM x, SCM y, SCM state);
void *register_scm_functions(void* data);

#endif
