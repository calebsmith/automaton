#ifndef BOARD_H
#define BOARD_H

#define BOARD_BORDER_SIZE 10

typedef struct {
    int toroidal;
    int width, height;
    int display_x, display_y;
    int display_width, display_height;
    int size;
    int *cells;
} Board_t;

void board_init(Board_t* board, const char* filename, int toroidal);
void board_copy(Board_t* board_a, const Board_t* board_b);
void board_swap(Board_t* board_a, Board_t* board_b);
int board_get_cell(const Board_t* board, int x, int y);
int board_count_moore_neighbors(const Board_t* board, int x, int y);
void board_destroy(Board_t* board);
#endif
