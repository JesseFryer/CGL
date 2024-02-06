/*
 * cgl.h
 *
 */

#ifndef CGL_H
#define CGL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "clm.h"
#include "camera.h"
#include "input.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "voxel_renderer.h"

/* cgl_init
 * --------
 * Initialise the cgl engine.
 */
bool cgl_init();

/* cgl_win_w
 * ---------
 * Returns the current window width.
 */
int cgl_win_w();

/* cgl_win_h
 * ---------
 * Returns the current window height.
 */
int cgl_win_h();

/* cgl_camera
 * ----------
 * Returns a pointer to the camera. See camera.h on how
 * to use it.
 */
Camera* cgl_camera();

/* cgl_window
 * ----------
 * Returns the window handle.
 */
GLFWwindow* cgl_window();

#endif
