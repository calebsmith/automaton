#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int toroidal;
    long long int sleep_time;
    int width, height, size;
    int *cells;
} board_t;

void board_init(board_t* board, const char* filename);
void board_copy(board_t* board_a, const board_t* board_b);
void board_swap(board_t* board_a, board_t* board_b);
int board_get_cell(const board_t* board, int x, int y);
int board_count_neighbors(const board_t* board, int x, int y);
void board_destroy(board_t* board);
#endif
