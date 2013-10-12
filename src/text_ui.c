#include "text_ui.h"


/*
 * initializes and clears a curses window with raw input mode, and no echo.
 *
 * parameters: void
 * returns: window* referring to the curses window
 */
WINDOW* init_curses(void)
{
    // create and prepare a curses window
    WINDOW* window = initscr();
    nodelay(window, true);
    cbreak();
    noecho();
    curs_set(0);
    clear();
    return window;
}


void main_curses(Board_t* board, Board_t* next_board, unsigned long long int sleep_time) {
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long last_time = 0;
    int playing = 1;    // Is the simulation "playing" or "paused"?
    int running = 1;    // Should the simulation continue to run?
    int current_char;   // What is the current keyboard input?

    // Initialize frontend
    WINDOW* window = init_curses();

    Lens_t lens;
    lens_init(&lens, board);

    // Display game board, find next generation, wait for time and loop
    while(running) {
        current_char = getch();
        if (current_char == 27 || current_char == 'q') {
            running = 0;
        }
        if (current_char == 'p') {
            playing = 0;
        }
        if (current_char == 'r') {
            playing = 1;
        }
        if (current_char == 'h') {
            lens_move_left(&lens, board);
        }
        if (current_char == 'l') {
            lens_move_right(&lens, board);
        }
        if (current_char == 'j') {
            lens_move_down(&lens, board);
        }
        if (current_char == 'k') {
            lens_move_up(&lens, board);
        }
        if (playing) {
            display_curses(board, &lens, window);
            generate(next_board, board);
            wait(sleep_time, &last_time);
        }
    }

    // Destroy curses
    endwin();
}

/*
 * Displays the current board in a curses window
 *
 * Parameters: Board_t* board, WINDOW* display_area
 * Return: void
 */
void display_curses(const Board_t* board, Lens_t* lens, WINDOW* window)
{
    int x, y;
    int display_x, display_y;
    int display_width, display_height;
    int value;

    // determine terminal's rows and columns. Display cell if in bounds
    getmaxyx(window, display_height, display_width);
    lens_set(lens, board, display_width, display_height);
    // clear the display of the old frame
    clear();
    for(y = lens->min_y; y < lens->max_y; y++) {
        for(x = lens->min_x; x < lens->max_x; x++) {
            value = board_get_cell(board, x, y);
            // Adjust display coordinates by the BOARD_BORDER_SIZE offset
            display_x = x + lens->x_display_offset;
            display_y = y + lens->y_display_offset;
            move(display_y, display_x);
            // display o for each living cell within the terminal
            if (display_y < display_height && display_x < display_width) {
                if (value != 0) {
                    insch('o');
                }
            }
        }
    }
    refresh();
}


