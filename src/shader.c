#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_FILE_SIZE 1000

char* read_shader_file(const char* filePath) {

    FILE* srcFile = fopen(filePath, "r");
    if (!srcFile) {
        fprintf(stderr,
                "couldn't open shader file\n");
        return NULL;
    }
    char* src = (char*) malloc(MAX_FILE_SIZE * sizeof(char));
    size_t srcIndex = 0;
    char tok;

    while ((tok = fgetc(srcFile)) != EOF) {

        src[srcIndex++] = tok;
        if (srcIndex >= MAX_FILE_SIZE) {
            fprintf(stderr, 
                    "exceeded shader file size limit\n");
            return NULL;
        }

    }

    fclose(srcFile);
    src[srcIndex] = '\0';
    return src;

}

unsigned int shader_create(
        const char* vShaderPath,
        const char* fShaderPath) {

    char* vShaderSrc = read_shader_file(vShaderPath);
    char* fShaderSrc = read_shader_file(fShaderPath);

    unsigned int vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    free(vShaderSrc);
    glCompileShader(vShader);

    unsigned int fShader;
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    free(fShaderSrc);
    glCompileShader(fShader);

    unsigned int shader;
    shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return shader;

}
