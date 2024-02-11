/*
 * chunk.h
 *
 */

#ifndef _CHUNK_H
#define _CHUNK_H

#include "types.h"

#define CHUNK_W    8  // x
#define CHUNK_H    32 // y
#define CHUNK_D    8  // z
#define VOX_PER_CHUNK (CHUNK_W * CHUNK_H * CHUNK_D)

typedef struct {
    u8 type;
} Voxel;

typedef struct {
    float x;
    float z;
    Voxel voxels[VOX_PER_CHUNK];
} Chunk;

/* chunk_render
 * ------------
 * Render a chunk.
 */
void chunk_render(Chunk* chunk);

/* chunk_idx_to_x
 * --------------
 * Convert and index in a 3d array to an x
 * coordinate.
 */
float chunk_idx_to_x(size_t idx);

/* chunk_idx_to_y
 * --------------
 * Convert and index in a 3d array to an y
 * coordinate.
 */
float chunk_idx_to_y(size_t idx);

/* chunk_idx_to_z
 * --------------
 * Convert and index in a 3d array to an z
 * coordinate.
 */
float chunk_idx_to_z(size_t idx);

/* chunk_gen_chunk
 * ---------------
 * Fill chunk with voxel information.
 */
void chunk_gen_chunk(
        Chunk* chunk, 
        float chunkX, 
        float chunkZ);

/* chunk_get_block
 * ---------------
 * Return the block type at the given position.
 */
u8 chunk_get_block(float x, float y, float z);

#endif
