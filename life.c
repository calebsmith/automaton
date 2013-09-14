#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int toroidal;
    long long int sleep_time;
    int width, height, size;
    int *cells;
} board_t;

void board_init(board_t* board, const char* filename);
void board_copy(board_t* board_a, board_t* board_b);
void board_swap(board_t* board_a, board_t* board_b);
int board_get_cell(board_t* board, int x, int y);
int board_count_neighbors(board_t* board, int x, int y);
void board_destroy(board_t* board);

void generate(board_t* next_board, board_t* board);
void display(board_t* board);


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Must provide a filename to a data file\n");
        exit(1);
    }
    const char* filename = argv[1];
    // Create a game board, and a next board
    board_t board, next_board;
    board_init(&board, filename);
    board_copy(&next_board, &board);

    // Display game board, find next generation and loop
    while(1) {
        display(&board);
        generate(&next_board, &board);
        usleep(board.sleep_time);
    }
    board_destroy(&board);
    board_destroy(&next_board);
    return 0;
}


void display(board_t* board) {
    int x, y;
    int value;

    for(y = 0; y < board->height; y++) {
        for(x = 0; x < board->width; x++) {
            value = board_get_cell(board, x, y);
            if (value) {
                printf("O");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}


void generate(board_t* next_board, board_t* board) {
    int x, y;
    int index;
    int num_neighbors;
    int current_cell;
    int x_lower_bound, x_higher_bound;
    int y_lower_bound, y_higher_bound;

    if (board->toroidal) {
        x_lower_bound = 0;
        y_lower_bound = 0;
        x_higher_bound = board->width;
        y_higher_bound = board->height;
    } else {
        x_lower_bound = 1;
        y_lower_bound = 1;
        x_higher_bound = board->width - 1;
        y_higher_bound = board->height - 1;
    }
    for(y = y_lower_bound; y < y_higher_bound; y++) {
        for(x = x_lower_bound; x < x_higher_bound; x++) {
            num_neighbors = board_count_neighbors(board, x, y);
            index = y * board->width + x;
            current_cell = board_get_cell(board, x, y);
            if (current_cell) {
                if (num_neighbors < 2) {
                    // underpopulation
                    next_board->cells[index] = 0;
                } else if (num_neighbors == 2 || num_neighbors == 3) {
                    // healthy living
                    next_board->cells[index] = 1;
                } else if (num_neighbors > 3) {
                    // overcrowding
                    next_board->cells[index] = 0;
                }
            } else {
                if (num_neighbors == 3) {
                    // reproduction
                    next_board->cells[index] = 1;
                } else {
                    // stasis
                    next_board->cells[index] = current_cell;
                }
            }
        }
    }
    board_swap(board, next_board);
}

void board_init(board_t* board, const char* filename)
{
    int i = 0;
    int current_cell;
    FILE *infile;

    if ((infile = fopen(filename, "r")) == NULL) {
        printf("Could not open file %s\n", filename);
        exit(1);
    } else {
        if((fscanf(infile, "%d", &board->toroidal)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%lld", &board->sleep_time)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%d", &board->width)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        if((fscanf(infile, "%d", &board->height)) != 1) {
            printf("Bad file format\n");
            exit(1);
        }
        board->size = board->width * board->height;
        board->cells = (int *) malloc(board->size * sizeof(int));
        while((current_cell = fgetc(infile)) != EOF) {
            if (i > board->size) {
                printf("File contains too many values\n");
                exit(2);
            }
            current_cell -= '0';
            if (current_cell == 0 || current_cell == 1) {
                board->cells[i++] = current_cell;
            }
        }
    }
    fclose(infile);
}

void board_copy(board_t* board_a, board_t* board_b)
{
    int i;

    board_a->toroidal = board_b->toroidal;
    board_a->sleep_time = board_b->sleep_time;
    board_a->width = board_b->width;
    board_a->height = board_b->height;
    board_a->size = board_b->size;
    board_a->cells = (int *) malloc(board_a->size * sizeof(int));
    for(i = 0; i< board_b->size; i++) {
        board_a->cells[i] = board_b->cells[i];
    }
}


void board_swap(board_t* board_a, board_t* board_b)
{
    int* swap_cells;
    swap_cells = board_a->cells;
    board_a->cells = board_b->cells;
    board_b->cells = swap_cells;
}


int board_get_cell(board_t* board, int x, int y)
{
    int index;
    if (x >= board->width) {
        x -= board->width;
    }
    if (x < 0) {
        x += board->width;
    }
    if (y >= board->height) {
        y -= board->height;
    }
    if (y < 0) {
        y += board->height;
    }
    index = y * board->width + x;
    return board->cells[index];
}

int board_count_neighbors(board_t* board, int x, int y)
{
    return (
        board_get_cell(board, x - 1, y - 1) +
        board_get_cell(board, x, y - 1) +
        board_get_cell(board, x + 1, y - 1) +
        board_get_cell(board, x - 1, y) +
        board_get_cell(board, x + 1, y) +
        board_get_cell(board, x - 1, y + 1) +
        board_get_cell(board, x, y + 1) +
        board_get_cell(board, x + 1, y + 1)
    );
}

void board_destroy(board_t* board)
{
    free(board->cells);
}
