#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glfw.h>
#include <GL/gl.h>

#include "board.h"
#include "algo.h"
#include "graphical_ui.h"


bool running = true;


int init_glfw(bool fullscreen) {

    int glfw_flag = (fullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;

    /* Initialize GLFW */
    if (!glfwInit()) {
        return GL_WINDOW_EXIT;
    }

    /* Create a windowed mode window and its OpenGL context */
    if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, glfw_flag)) {
        glfwTerminate();
        return GL_WINDOW_EXIT;
    }
    glfwSetWindowTitle("Game of Life");
    glfwSwapInterval(0);
    glfwEnable(GLFW_AUTO_POLL_EVENTS);
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);

    // Register callbacks
    glfwSetWindowCloseCallback(*handle_window_close);
    glfwSetKeyCallback(*handle_escape);
    return 0;
}


void main_glfw(Board_t* board, Board_t* next_board, unsigned long long int sleep_time, bool fullscreen) {
    // Initialize the `last_time` variable for the real-time clock. Tracks the
    // time the last loop began for calculating time to wait.
    unsigned long long last_time = 0;

    // Initialize frontend
    init_glfw(fullscreen);

    // Display game board, find next generation, wait for time and loop
    while(running) {
        render(board);
        generate(next_board, board);
        wait(sleep_time, &last_time);
    }

    // Destroy glfw
    glfwCloseWindow();
    glfwTerminate();
}


void GLFWCALL handle_escape(int key, int action)
{
    if (key == GLFW_KEY_ESC) {
        running = false;
    }
}


int GLFWCALL handle_window_close(void)
{
    running = false;
    return GL_FALSE;
}

void render(Board_t* board) {
    float x, y;
    int display_x, display_y;
    int value;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS );
    for(y = board->display_y; y < board->display_height; y++) {
        for(x = board->display_x; x < board->display_width; x++) {
            value = board_get_cell(board, x, y);
            display_x = (!board->toroidal) ? x - BOARD_BORDER_SIZE : x;
            display_y = (!board->toroidal) ? y - BOARD_BORDER_SIZE : y;
            if (value != 0) {
                make_quad(display_x, display_y, 4);
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
