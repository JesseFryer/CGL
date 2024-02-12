#include "chunk.h"
#include "voxel_renderer.h"
#include "clm.h"
#include "perlin.h"
#include <stdio.h>

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
    float chunkZ = chunk->z * CHUNK_D;

    for (int z = 0; z < CHUNK_D; z++) {
        for (int y = 0; y < CHUNK_H; y++) {
            for (int x = 0; x < CHUNK_W; x++) {
                size_t idx = chunk_pos_to_idx(x, y, z);
                Voxel* voxel = &chunk->voxels[idx];

                // Skip air.
                if (!voxel->type) {
                    continue;
                }

                // Infer position of voxel.
                position[0] = chunkX + x;
                position[1] = y;
                position[2] = chunkZ + z;

                // Submit voxel.
                voxren_submit_vox(
                        position, 
                        size,
                        colour,
                        &tex);
            }
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

size_t chunk_pos_to_idx(int x, int y, int z) {
    return (size_t) (z * CHUNK_W * CHUNK_H) + (y * CHUNK_W) + x;
}

void chunk_gen_chunk(
        Chunk* chunk, 
        float chunkX, 
        float chunkZ) {
    // Set the chunk coords.
    chunk->x = chunkX;
    chunk->z = chunkZ;

    // Determine which type each voxel is.
    for (size_t i = 0; i < VOX_PER_CHUNK; i++) {
        // Get world voxel coords.
        float x = (chunkX * CHUNK_W) + chunk_idx_to_x(i);
        float y = chunk_idx_to_y(i);
        float z = (chunkZ * CHUNK_D) + chunk_idx_to_z(i);

        float var = 2.0f * perlin2d(x, z, 0.02f, 1);
        float surfaceY = (CHUNK_H / 2) + var;

        // Set block type.
        if (y < surfaceY) {
            chunk->voxels[i].type = chunk_get_block(
                    x, y, z);
        } else {
            chunk->voxels[i].type = 0;
        }
    }
}

u8 chunk_get_block(float x, float y, float z) {
    return 1; // Grass.
}
