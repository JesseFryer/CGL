#include "voxel_renderer.h"
#include "vao.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct RenderData {
    VAO          vao;
    unsigned int vbo;
    unsigned int shader;
    Vertex*      vBuffer;
    Vertex*      vBufferPtr;
    GLsizei      indicesCount;
} RenderData;

static RenderData s_rData;

void voxren_init() {
    // Setup vbo.
    glGenBuffers(1, &s_rData.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_rData.vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            MAX_VERTICES * sizeof(Vertex),
            NULL,
            GL_DYNAMIC_DRAW);

    // Setup vertex buffer.
    s_rData.vBuffer = (Vertex*) malloc(MAX_VERTICES * sizeof(Vertex));
    s_rData.vBufferPtr = s_rData.vBuffer;
    s_rData.indicesCount = 0;

    // Setup our vao and ebo layout.
    vao_init(&s_rData.vao);
    // Position.
    vao_vertex_attrib(
            &s_rData.vao,
            0,              // attrib no.
            3,              // no of elements in attrib.
            GL_FLOAT,       // type of the elements.
            GL_FALSE,       // normalise.
            sizeof(Vertex), // stride.
            (void*) offsetof(Vertex, position));
    // Colour.
    vao_vertex_attrib(
            &s_rData.vao,
            1,              // attrib no.
            4,              // no of elements in attrib.
            GL_FLOAT,       // type of the elements.
            GL_FALSE,       // normalise.
            sizeof(Vertex), // stride.
            (void*) offsetof(Vertex, colour));
    // Normal.
    vao_vertex_attrib(
            &s_rData.vao,
            2,              // attrib no.
            3,              // no of elements in attrib.
            GL_FLOAT,       // type of the elements.
            GL_FALSE,       // normalise.
            sizeof(Vertex), // stride.
            (void*) offsetof(Vertex, normal));

    // Generate indices buffer and send it to GPU.
    uint32_t indices[MAX_INDICES];
    uint32_t offset = 0;
    for (size_t i = 0; i < MAX_INDICES; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 0;
        indices[i + 5] = offset + 3;

        offset += 4;
    }

    // Give ebo the indices array. This stays here
    // forever, only need to change how many we use.
    vao_ebo_data(
            &s_rData.vao,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW);

}

void voxren_submit_vox(
        clmVec3 position, 
        clmVec3 size,
        clmVec4 colour) {
    // For a voxel, 24 vertices are used, 4 for each rectangular face.
    // They are added in anti-clockwise order, so bottom left/right then
    // top right/left.
   
    // When the buffers are full flush the batch.
    if (s_rData.indicesCount >= MAX_INDICES) {
        voxren_render_batch();
    }

    // Unpack frequently used info.
    float x = position[0];
    float y = position[1];
    float z = position[2];

    float width  = size[0] * 0.5f;
    float height = size[1] * 0.5f;
    float depth  = size[2] * 0.5f;

    float r = colour[0];
    float g = colour[1];
    float b = colour[2];
    float a = colour[3];

    // Front face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 1.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 1.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 1.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 1.0f;
    s_rData.vBufferPtr++;


    // Right face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 1.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 1.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 1.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] = width  + x;
    s_rData.vBufferPtr->position[1] = height + y;
    s_rData.vBufferPtr->position[2] = depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 1.0f;
    s_rData.vBufferPtr->normal[1] = 0.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;


    // Back face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] = -1.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] = -1.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] = -1.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] = -1.0f;
    s_rData.vBufferPtr++;


    // Left face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = -1.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = -1.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = -1.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = -1.0f;
    s_rData.vBufferPtr->normal[1] =  0.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;


    // Bottom face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] = -1.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] = -1.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] = -1.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] = -height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] =  0.0f;
    s_rData.vBufferPtr->normal[1] = -1.0f;
    s_rData.vBufferPtr->normal[2] =  0.0f;
    s_rData.vBufferPtr++;


    // Top face of voxel.
    // Bottom-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 1.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Bottom-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] =  depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 1.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Top-right.
    s_rData.vBufferPtr->position[0] =  width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 1.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;
    // Top-left.
    s_rData.vBufferPtr->position[0] = -width  + x;
    s_rData.vBufferPtr->position[1] =  height + y;
    s_rData.vBufferPtr->position[2] = -depth  + z;
    s_rData.vBufferPtr->colour[0] = r;
    s_rData.vBufferPtr->colour[1] = g;
    s_rData.vBufferPtr->colour[2] = b;
    s_rData.vBufferPtr->colour[3] = a;
    s_rData.vBufferPtr->normal[0] = 0.0f;
    s_rData.vBufferPtr->normal[1] = 1.0f;
    s_rData.vBufferPtr->normal[2] = 0.0f;
    s_rData.vBufferPtr++;

    s_rData.indicesCount += INDICES_PER_VOXEL;
}

void voxren_render_batch() {
    // Bind vao and use shader.
    vao_bind(&s_rData.vao);

    // Bind vbo and supply vertices.
    GLsizeiptr size = 
        (uint8_t*) s_rData.vBufferPtr - (uint8_t*) s_rData.vBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, s_rData.vbo);
    glBufferSubData(
            GL_ARRAY_BUFFER, 
            0,
            size,
            s_rData.vBuffer);

    // Render draw call.
    glDrawElements(
            GL_TRIANGLES,
            s_rData.indicesCount,
            GL_UNSIGNED_INT,
            0);

    // Reset indices count and vBufferPtr.
    s_rData.indicesCount = 0;
    s_rData.vBufferPtr = s_rData.vBuffer;
}

void voxren_set_shader(unsigned int shader) {
    s_rData.shader = shader;
}

void voxren_terminate() {
    free(s_rData.vBuffer);
}
