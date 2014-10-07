#include "scm.h"

World_t world_g;


SCM scm_von_neumann_neighbors(SCM x_in, SCM y_in, SCM state_in)
{
    int x = scm_to_int(x_in);
    int y = scm_to_int(y_in);
    unsigned char state = scm_to_uchar(state_in);

    return scm_from_int(
        board_count_von_neumann_neighbors(world_g.board, x, y, state));
}

SCM scm_moore_neighbors(SCM x_in, SCM y_in, SCM state_in)
{
    int x = scm_to_int(x_in);
    int y = scm_to_int(y_in);
    unsigned char state = scm_to_uchar(state_in);

    return scm_from_int(
        board_count_moore_neighbors(world_g.board, x, y, state));
}

SCM scm_get_width()
{
    return scm_from_int(world_g.board->width);
}

SCM scm_get_height()
{
    return scm_from_int(world_g.board->height);
}

SCM scm_get_toroidal()
{
    return scm_from_int(board_get_toroidal(world_g.board));
}

SCM scm_get_cell(SCM x, SCM y)
{
    return scm_from_int(
        board_get_cell(world_g.board, scm_to_int(x), scm_to_int(y)));
}

SCM scm_board_done()
{
    board_copy(world_g.board, world_g.next_board);
    return scm_from_int(0);
}


SCM scm_set_cell(SCM x_in, SCM y_in, SCM state_in)
{
    int x = scm_to_int(x_in);
    int y = scm_to_int(y_in);
    unsigned char state = scm_to_uchar(state_in);

    board_set_cell(world_g.next_board, x, y, state);
    return scm_from_int(0);
}

void *register_scm_functions(void* data)
{
    scm_c_define_gsubr("board-set-cell", 3, 1, 0, &scm_set_cell);
    scm_c_define_gsubr("board-get-cell", 2, 0, 0, &scm_get_cell);
    scm_c_define_gsubr("board-von-neumann-neighbors", 3, 0, 0, &scm_von_neumann_neighbors);
    scm_c_define_gsubr("board-moore-neighbors", 3, 0, 0, &scm_moore_neighbors);
    scm_c_define_gsubr("board-get-width", 0, 0, 0, &scm_get_width);
    scm_c_define_gsubr("board-get-height", 0, 0, 0, &scm_get_height);
    scm_c_define_gsubr("board-toroidal?", 0, 0, 0, &scm_get_toroidal);
    scm_c_define_gsubr("board-done!", 0, 0, 0, &scm_board_done);
    return NULL;
}

void scm_generate(World_t* world)
{
    SCM func = world->rule->scm_cell_func;

    scm_call_0(func);
}
