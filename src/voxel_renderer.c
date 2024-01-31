#include "voxel_renderer.h"
#include "vao.h"

typedef struct RenderData {
    VAO vao;
    unsigned int vbo;
    unsigned int indices[MAX_INDICES];
} RenderData;

static RenderData s_rData;

void voxren_init() {
    // Setup our vao and ebo.
    vao_init(&s_rData.vao);
    vao_vertex_attrib(
            &s_rData.vao,
            0,              // attrib no.
            3,              // no of elements in attrib.
            GL_FLOAT,       // type of the elements.
            GL_FALSE,       // normalise.
            sizeof(Vertex), // stride.
            (void*) offsetof(Vertex, position));

    // Generate indices buffer and send it to GPU.
    unsigned int* indices = s_rData.indices;
    size_t indicesIndex = 0;
    for (unsigned int voxel = 0; voxel < MAX_VOXELS; voxel++) {
        unsigned int startIndex = voxel * 24;
        // We need to fill out 6 quads of indices to make the voxel.
        indices[indicesIndex++] = startIndex + 0;
        indices[indicesIndex++] = startIndex + 1;
        indices[indicesIndex++] = startIndex + 2;
        indices[indicesIndex++] = startIndex + 2;
        indices[indicesIndex++] = startIndex + 0;
        indices[indicesIndex++] = startIndex + 3;

        indices[indicesIndex++] = startIndex + 4;
        indices[indicesIndex++] = startIndex + 5;
        indices[indicesIndex++] = startIndex + 6;
        indices[indicesIndex++] = startIndex + 6;
        indices[indicesIndex++] = startIndex + 4;
        indices[indicesIndex++] = startIndex + 7;

        indices[indicesIndex++] = startIndex + 8;
        indices[indicesIndex++] = startIndex + 9;
        indices[indicesIndex++] = startIndex + 10;
        indices[indicesIndex++] = startIndex + 10;
        indices[indicesIndex++] = startIndex + 8;
        indices[indicesIndex++] = startIndex + 11;

        indices[indicesIndex++] = startIndex + 12;
        indices[indicesIndex++] = startIndex + 13;
        indices[indicesIndex++] = startIndex + 14;
        indices[indicesIndex++] = startIndex + 14;
        indices[indicesIndex++] = startIndex + 12;
        indices[indicesIndex++] = startIndex + 15;

        indices[indicesIndex++] = startIndex + 16;
        indices[indicesIndex++] = startIndex + 17;
        indices[indicesIndex++] = startIndex + 18;
        indices[indicesIndex++] = startIndex + 18;
        indices[indicesIndex++] = startIndex + 16;
        indices[indicesIndex++] = startIndex + 19;

        indices[indicesIndex++] = startIndex + 20;
        indices[indicesIndex++] = startIndex + 21;
        indices[indicesIndex++] = startIndex + 22;
        indices[indicesIndex++] = startIndex + 22;
        indices[indicesIndex++] = startIndex + 20;
        indices[indicesIndex++] = startIndex + 23;
    }

}
