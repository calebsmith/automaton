/*
 * A graphical front-end that handles input and displays the board in an OpenGL
 * context of a window using the glfw library. This relies on OpenGL
 * extensions, but should be somewhat portable since glfw handles input/window
 * handling for GNU/Linux, MacOSX, and Windows
 */
#ifndef GRAPHICAL_UI_H
#define GRAPHICAL_UI_H

#include <stdbool.h>

#include <GLFW/glfw3.h>

#include "world.h"
#include "lens.h"
#include "backend.h"

#define WINDOW_WIDTH 1330
#define WINDOW_HEIGHT 768
#define GL_WINDOW_EXIT 4

int init_glfw(bool fullscreen);
int main_glfw(World_t* world, unsigned long long int sleep_time, bool fullscreen);
void render(World_t* world, Lens_t* lens);
void make_quad(float x, float y, float size, const Color_t* color);

void handle_keys(GLFWwindow* window, int key, int scancode, int action, int mods);
void handle_window_close(GLFWwindow* window);

#endif
