#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int toroidal;
    long long int sleep_time;
    int width, height;
    int *cells;
} Board_t;

void board_init(Board_t* board, const char* filename);
void board_copy(Board_t* board_a, const Board_t* board_b);
void board_swap(Board_t* board_a, Board_t* board_b);
int board_get_cell(const Board_t* board, int x, int y);
int board_count_neighbors(const Board_t* board, int x, int y);
void board_destroy(Board_t* board);
#endif
