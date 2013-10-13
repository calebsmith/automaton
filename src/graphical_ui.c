#include "graphical_ui.h"


bool running = true;
bool playing = true;
Lens_t lens;


int init_glfw(bool fullscreen) {

    int glfw_flag = (fullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;

    /* Initialize GLFW */
    if (!glfwInit()) {
        return GL_WINDOW_EXIT;
    }

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, glfw_flag)) {
        return GL_WINDOW_EXIT;
    }
    glfwSetWindowTitle("Game of Life");
    glfwSwapInterval(0);
    glfwDisable(GLFW_AUTO_POLL_EVENTS);
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);

    // Register callbacks
    glfwSetWindowCloseCallback(*handle_window_close);
    glfwSetKeyCallback(*handle_keys);
    return 0;
}


int main_glfw(Board_t* board, Board_t* next_board, unsigned long long int sleep_time, bool fullscreen) {
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long last_time = 0;
    int return_value = 0;

    lens_init(&lens, board);

    // Display game board, find next generation, wait for time and loop
    if (!init_glfw(fullscreen)) {
        while (running) {
            glfwPollEvents();
            render(board, &lens);
            if (playing) {
                generate(next_board, board);
                wait(sleep_time, &last_time);
            }
        }
    } else {
        fprintf(stderr, "Failed to open a window for OpenGL rendering\n");
        return_value = GL_WINDOW_EXIT;
    }

    // Destroy glfw
    glfwCloseWindow();
    glfwTerminate();
    return return_value;
}


void GLFWCALL handle_keys(int key, int action)
{
    switch(key) {
        case GLFW_KEY_ESC:
        case 'Q':
            running = false;
            break;
        case 'P':
            playing = false;
            break;
        case 'R':
            playing = true;
            break;
        case 'H':
        case GLFW_KEY_LEFT:
            lens_move_left(&lens);
            break;
        case 'J':
        case GLFW_KEY_DOWN:
            lens_move_down(&lens);
            break;
        case 'K':
        case GLFW_KEY_UP:
            lens_move_up(&lens);
            break;
        case 'L':
        case GLFW_KEY_RIGHT:
            lens_move_right(&lens);
            break;
    }
}


int GLFWCALL handle_window_close(void)
{
    running = false;
    return GL_FALSE;
}

void render(Board_t* board, Lens_t* lens) {
    float x, y;
    int display_x, display_y;
    int value;

    lens_set(lens, board, WINDOW_WIDTH, WINDOW_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    for(y = lens->min_y; y < lens->max_y; y++) {
        for(x = lens->min_x; x < lens->max_x; x++) {
            value = board_get_cell(board, x, y);
            display_x = x - lens->x_display_offset;
            display_y = y - lens->y_display_offset;
            if (value != 0) {
                make_quad(display_x, display_y, lens->scale);
            }
        }
    }
    glEnd();
    glfwSwapBuffers();
}

inline void make_quad(float x, float y, float size) {
    // To change color uncomment
    // glColor3f(255.0f, 0.0f, 255.0f);
    //top-left vertex (corner)
    glTexCoord2i(0, 0);
    glVertex3f(x * size, y * size, 0.0f);
    //top-right vertex (corner)
    glTexCoord2i(1, 0);
    glVertex3f(size + x * size, y * size, 0.f);
    //bottom-right
    glTexCoord2i(1, 1);
    glVertex3f(size + x * size, size + y * size, 0.f);
    //bottom-left
    glTexCoord2i(0, 1);
    glVertex3f(x * size, size + y * size, 0.f);
}
