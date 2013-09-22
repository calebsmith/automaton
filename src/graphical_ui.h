#ifndef GRAPHICAL_UI_H
#define GRAPHICAL_UI_H

#include <stdbool.h>

#include <GL/glfw.h>
#include <GL/gl.h>

#include "board.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GL_WINDOW_EXIT 4

void GLFWCALL handle_escape(int key, int action);
int GLFWCALL handle_window_close(void);
int init_glfw(void);
void main_glfw(Board_t* board, Board_t* next_board, unsigned long long int sleep_time);
void render(Board_t* board);
void make_quad(float x, float y, float size);

#endif
