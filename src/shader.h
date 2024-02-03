/* shader.h
 */

/* create_shader
 * -------------
 * Returns the ID reference to the shader program.
 */

#ifndef SHADER_H
#define SHADER_H

#include "clm.h"

#define SHADER_FOLDER "../src/shaders/"

/* shader_create
 * -------------
 * Returns id handle of shader program.
 */
unsigned int shader_create(
        const char* vShaderPath,
        const char* fShaderPath);

/* shader_use
 * ----------
 * Sets the given shader program handle as the currently used
 * shader.
 */
void shader_use(unsigned int shader);

/* shader_set_uniform_mat4
 * -----------------------
 * Set a mat4 uniform in shader.
 */
void shader_set_uniform_mat4(
        unsigned int shader,
        const char* uniformName,
        clmMat4 mat);

/* shader_set_uniform_vec3
 * -----------------------
 * Set a vec3 uniform in shader.
 */
void shader_set_uniform_vec3(
        unsigned int shader,
        const char* uniformName,
        clmVec3 vec);

#endif
