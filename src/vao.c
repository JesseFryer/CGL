#include "vao.h"

void vao_init(VAO* vao) {
    // Create vao/ebo
    glGenVertexArrays(1, &vao->id);
    glGenBuffers(1, &vao->ebo);

    vao_bind(vao);

    // Attach the ebo.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->ebo);

    vao_unbind();
}

void vao_bind(VAO* vao) {
    glBindVertexArray(vao->id);
}

void vao_unbind() {
    glBindVertexArray(0);
}

void vao_ebo_data(
        VAO* vao,
        GLsizeiptr size,
        const GLvoid* data,
        GLenum usage,
        unsigned int indicesCount) {
    vao->indicesCount = indicesCount;

    vao_bind(vao);

    // Give ebo the data.
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            size,
            data,
            usage);

    vao_unbind();
}

void vao_vertex_attrib(
        VAO* vao,
        GLuint index,
        GLint  size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid* ptr) {

    vao_bind(vao);

    // Specify the attribute.
    glVertexAttribPointer(
            index,
            size,
            type,
            normalized,
            stride,
            ptr);

    // Enable it.
    glEnableVertexAttribArray(index);

    vao_unbind();

}

