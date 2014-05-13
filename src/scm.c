#include "scm.h"


void scm_handle_cell(const World_t* world, int cell_x, int cell_y)
{
    SCM func;
    SCM cell_value;
    SCM cell_list;
    SCM current_cell;
    int x, y;
    unsigned char result;

    /* Create a Scheme list of ints for get-cell to use */
    cell_list = scm_list_n(SCM_UNDEFINED);
    for (y = cell_y - 1; y <= cell_y + 1; y++) {
        for (x = cell_x - 1; x <= cell_x + 1; x++) {
            cell_value = scm_from_uchar(board_get_cell(world->board, x, y));
            cell_list = scm_append_x(
                scm_list_n(cell_list,
                    scm_list_n(cell_value, SCM_UNDEFINED), SCM_UNDEFINED));
        }
    }
    func  = scm_variable_ref(scm_c_public_lookup("gameoflife", "get-next-cell"));
    current_cell = scm_from_uchar(board_get_cell(world->board, cell_x, cell_y));
    result = scm_to_uchar(scm_call_2(func, cell_list, current_cell));
    world->next_board->cells[(cell_y * world->board->width + cell_x)] = result;
}

void *register_scm_functions(void* data)
{
    return NULL;
}
