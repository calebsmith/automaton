#include "graphical_ui.h"


bool running = true;
bool playing = true;
Lens_t lens;
GLFWwindow* window;


int init_glfw(bool fullscreen) {

    GLFWmonitor* monitor;

    /* Initialize GLFW */
    if (!glfwInit()) {
        return GL_WINDOW_EXIT;
    }

    monitor = (fullscreen) ? glfwGetPrimaryMonitor(): NULL;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
        "Game of Life", monitor, NULL);
    if (!window) {
        return GL_WINDOW_EXIT;
    };
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);

    // Register callbacks
    glfwSetWindowCloseCallback(window, *handle_window_close);
    glfwSetKeyCallback(window, *handle_keys);
    return 0;
}


int main_glfw(World_t* world, unsigned long long int sleep_time, bool fullscreen) {
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long last_time = 0;
    int return_value = 0;

    lens_init(&lens, world->board, WINDOW_WIDTH, WINDOW_HEIGHT, true);

    // Display game board, find next generation, wait for time and loop
    if (!init_glfw(fullscreen)) {
        while (running) {
            glfwPollEvents();
            render(world, &lens);
            if (playing) {
                generate(world);
                wait(sleep_time, &last_time);
            }
        }
    } else {
        fprintf(stderr, "Failed to open a window for OpenGL rendering\n");
        return_value = GL_WINDOW_EXIT;
    }

    // Destroy glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return return_value;
}


void handle_keys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
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
            case 'I':
                lens_zoom_in(&lens);
                break;
            case 'O':
                lens_zoom_out(&lens);
                break;
        }
    }
}


void handle_window_close(GLFWwindow* window)
{
    running = false;
}

void render(const World_t* world, Lens_t* lens) {
    float x, y;
    int display_x, display_y;
    int value;

    lens_set(lens, world->board, WINDOW_WIDTH, WINDOW_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    for(y = lens->min_y; y < lens->max_y; y++) {
        for(x = lens->min_x; x < lens->max_x; x++) {
            value = board_get_cell(world->board, x, y);
            display_x = x - lens->x_display_offset;
            display_y = y - lens->y_display_offset;
            if (value != 0) {
                make_quad(
                    display_x, display_y, lens->scale,
                    &(world->rule->state_colors[value])
                );
            }
        }
    }
    glEnd();
    glfwSwapBuffers(window);
}

inline void make_quad(float x, float y, float size, const Color_t* color) {
    glColor3ub(color->red, color->green, color->blue);
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
