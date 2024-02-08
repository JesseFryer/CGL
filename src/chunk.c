#include "chunk.h"
#include "voxel_renderer.h"
#include "clm.h"

void chunk_render(Chunk* chunk) {
    clmVec3 position = { 0.0f };
    clmVec3 size = { 1.0f, 1.0f, 1.0f };
    clmVec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

    // TODO: Move atlas and Voxel tex stuff to cgl. 
    SpriteAtlas atlas;
    VoxelTex tex;
    tex_init_atlas(
            &atlas,
            256,
            256,
            16,
            16);
    tex_create_voxel_tex(
            &tex,
            &atlas,
            240,
            242,
            243);

    // Determine bottom left coordinates of chunk.
    float chunkX = chunk->x * CHUNK_W; 
    float chunkY = chunk->y * CHUNK_H;
    float chunkZ = chunk->z * CHUNK_D;

    for (size_t i = 0; i < BLOCKS_PER_CHUNK; i++) {
        // Infer position of voxel.
        if (chunk->blocks[i].type) {
            position[0] = chunkX + chunk_idx_to_x(i);
            position[1] = chunkY + chunk_idx_to_y(i);
            position[2] = chunkZ + chunk_idx_to_z(i);
            voxren_submit_vox(
                    position, 
                    size,
                    colour,
                    &tex);
        }
    }

    voxren_render_batch();
}

float chunk_idx_to_x(size_t idx) {
    return (float) (idx % CHUNK_W);
}

float chunk_idx_to_y(size_t idx) {
    if (!idx) {
        return 0.0f;
    }
    return (float) ((idx / CHUNK_W) % CHUNK_H); 
}

float chunk_idx_to_z(size_t idx) {
    return (float) ((idx / CHUNK_W) / CHUNK_H);
}

