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
#include "types.h"
#include "perlin.h"
#include "chunk.h"

typedef struct {
    GLFWwindow* window;
    Camera camera;
    clmMat4 proj;
    clmMat4 view;
    u32 voxelShader;
    u32 lightShader;
    Chunk chunks;
    int winW;
    int winH;
} AppData;

/* cgl_init
 * --------
 * Initialise the cgl engine.
 */
AppData* cgl_init();

/* cgl_run
 * -------
 * Main loop.
 */
void cgl_run();

/* cgl_cleanup
 * -----------
 * Cleanup resources.
 */
void cgl_cleanup();

/* cgl_update_view
 * ---------------
 * Recalculate the camera's view matrix and
 * update the corresponding uniforms in shaders.
 */
void cgl_update_view();

/* cgl_move_camera
 * --------------
 * Update the camera's position.
 */
void cgl_move_camera(float timeStep);

#endif
