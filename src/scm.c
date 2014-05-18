#include "scm.h"

World_t world;


SCM scm_neighbor_count(SCM x_in, SCM y_in, SCM state_in)
{
    int x = scm_to_int(x_in);
    int y = scm_to_int(y_in);
    unsigned char state = scm_to_uchar(state_in);
    return scm_from_int(
        world.rule->neighbor_func(world.board, x, y, state));
}

SCM scm_get_cell(SCM x, SCM y)
{
    return scm_from_uchar(
        board_get_cell(world.board, scm_to_int(x), scm_to_int(y)));
}

SCM scm_set_cell(SCM x_in, SCM y_in, SCM state)
{
    int x = scm_to_int(x_in);
    int y = scm_to_int(y_in);
    world.next_board->cells[(y * world.board->width + x)] = scm_to_uchar(state);
    return SCM_UNDEFINED;
}

void *register_scm_functions(void* data)
{
    scm_c_define_gsubr("board-neighbor-count", 3, 0, 0, &scm_neighbor_count);
    scm_c_define_gsubr("board-get-cell", 2, 0, 0, &scm_get_cell);
    scm_c_define_gsubr("board-set-cell", 3, 0, 0, &scm_set_cell);
    return NULL;
}

void scm_generate(World_t* world)
{
    SCM func;
    int x, y;

    func  = scm_variable_ref(scm_c_public_lookup("gameoflife", "get-next-cell"));
    /* Create a Scheme list of ints for get-cell to use */
    for (y = 0; y < world->board->height; y++) {
        for (x = 0; x < world->board->width; x++) {
            scm_call_2(func, scm_from_int(x), scm_from_int(y));
        }
    }
}
