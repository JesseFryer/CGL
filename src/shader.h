/* shader.h
 */

/* create_shader
 * -------------
 * Returns the ID reference to the shader program.
 */

#ifndef SHADER_H
#define SHADER_H

unsigned int shader_create(
        const char* vShaderPath,
        const char* fShaderPath);

#endif
