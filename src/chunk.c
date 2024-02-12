#include "chunk.h"
#include "voxel_renderer.h"
#include "clm.h"
#include "perlin.h"
#include <stdio.h>
#include <stdbool.h>

void chunk_render(Chunk* chunk) {
    clmVec3 position;
    clmVec3 size;
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

    // Greedy mesh this bihhhh.
    // Use this to keep track of which voxels have been 
    // meshed already.
    u8 meshedIndices[VOX_PER_CHUNK] = { 0 };

    for (int y = 0; y < CHUNK_H; y++) {
        for (int z = 0; z < CHUNK_D; z++) {
            for (int x = 0; x < CHUNK_W; x++) {
                size_t idx = chunk_pos_to_idx(x, y, z);
                // Skip meshed indices.
                if (meshedIndices[idx]) {
                    continue;
                }

                // Check this one off as meshed.
                meshedIndices[idx] = 1;

                Voxel* voxel = &chunk->voxels[idx];
                u8 voxType = voxel->type;

                // Skip air.
                if (!voxType) {
                    continue;
                }

                // Reset size from last mesh.
                size[0] = 1.0f;
                size[1] = 1.0f;
                size[2] = 1.0f;


                // Greedy mesh.
                //
                // X axis first.
                // Start at the next voxel.
                // Go until chunk border, or different
                // voxel type.
                for (int gx = x + 1; gx < CHUNK_W; gx++) {
                    // Get the next voxel.
                    size_t next = chunk_pos_to_idx(gx, y, z);
                    u8 nextType = chunk->voxels[next].type;
                    if (voxType == nextType) {
                        // Check off next voxel as meshed.
                        meshedIndices[next] = 1;
                        // Increase size of mesh.
                        size[0] += 1;
                    }
                }

                // Z axis next.
                // Start at the next voxel.
                // Go until chunk border, or different
                // voxel type.
                for (int gz = z + 1; gz < CHUNK_D; gz++) {
                    // size[0] amount of voxels must be
                    // the same along the x axis of each
                    // z index.

                    // Determine if we can expand this row.
                    bool canExpand = true;
                    for (int gx = x; gx < x + (int) size[0]; gx++) {
                        size_t next = chunk_pos_to_idx(gx, y, gz);
                        u8 nextType = chunk->voxels[next].type;
                        if (voxType != nextType) {
                            canExpand = false;
                            break;
                        }
                    }

                    if (canExpand) {
                        // Check off the row as meshed.
                        for (int gx = x; gx < x + (int) size[0]; gx++) {
                            size_t next = chunk_pos_to_idx(gx, y, gz);
                            meshedIndices[next] = 1;
                        }
                        // Increase size of mesh.
                        size[2] += 1;
                    } else {
                        break;
                    }
                }

                // Y axis next.
                // size[0] x size[2] amount of voxels must be
                // the same along the x-z axis of each
                // y index.
                bool canExpand = true;
                for (int gy = y + 1; gy < CHUNK_H; gy++) {
                    for (int gz = z; gz < z + size[2]; gz++) {
                        for (int gx = x; gx < x + size[0]; gx++) {
                            size_t next = chunk_pos_to_idx(gx, gy, gz);
                            u8 nextType = chunk->voxels[next].type;
                            if (voxType != nextType) {
                                canExpand = false;
                                break;
                            }
                        }
                        if (!canExpand) {
                            break;
                        }
                    }

                    if (canExpand) {
                        // Check off plane as meshed.
                        for (int gz = z; gz < z + size[2]; gz++) {
                            for (int gx = x; gx < x + size[0]; gx++) {
                                size_t next = chunk_pos_to_idx(gx, gy, gz);
                                meshedIndices[next] = 1;
                            }
                        }
                        // Increase size of mesh.
                        size[1] += 1;
                    } else {
                        break;
                    }
                }

                // After increasing size of our mesh,
                // Send it to gpu.
                
                // Infer world position of voxel.
                position[0] = chunkX + x + (size[0] * 0.5f);
                position[1] = y - 1 + (size[1] * 0.5f);
                position[2] = chunkZ + z + (size[2] * 0.5f);
               
                // Submit the voxel.
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

        float var = 1.0f * perlin2d(x, z, 0.02f, 1);
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
