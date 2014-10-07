/*
 */
#ifndef SCM_H
#define SCM_H


#include <libguile.h>

#include "world.h"

void scm_generate(World_t* world);
void *register_scm_functions(void* data);

#endif
