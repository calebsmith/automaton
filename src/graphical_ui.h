#ifndef GRAPHICAL_UI_H
#define GRAPHICAL_UI_H

#include <stdbool.h>

#include <GL/glfw.h>
#include <GL/gl.h>

#include "board.h"
#include "lens.h"
#include "backend.h"

#define WINDOW_WIDTH 1330
#define WINDOW_HEIGHT 768
#define GL_WINDOW_EXIT 4

void GLFWCALL handle_escape(int key, int action);
int GLFWCALL handle_window_close(void);
int init_glfw(bool fullscreen);
int main_glfw(Board_t* board, Board_t* next_board, unsigned long long int sleep_time, bool fullscreen);
void render(Board_t* board, Lens_t* lens);
void make_quad(float x, float y, float size);

#endif
