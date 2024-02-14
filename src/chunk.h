/*
 * chunk.h
 *
 */

#ifndef _CHUNK_H
#define _CHUNK_H

#include "types.h"
#include "clm.h"
#include <stdbool.h>

#define CHUNK_W    16  // x
#define CHUNK_H    256   // y
#define CHUNK_D    16  // z
#define VOX_PER_CHUNK (CHUNK_W * CHUNK_H * CHUNK_D)

#define CHUNK_GEN_D 24 // Diameter.
#define CHUNK_GEN_COUNT (CHUNK_GEN_D * CHUNK_GEN_D)

typedef struct {
    u8 type;
} Voxel;

typedef struct {
    clmVec3 positions[VOX_PER_CHUNK];
    clmVec3 sizes[VOX_PER_CHUNK];
    u16 voxelCount;
} Mesh;

typedef struct {
    float x;
    float z;
    Voxel voxels[VOX_PER_CHUNK];
    Mesh mesh;
    bool meshed;
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

/* chunk_pos_to_idx
 * --------------
 * Convert chunk position to index in 1D array.
 */
size_t chunk_pos_to_idx(int x, int y, int z);

/* chunk_gen_chunk
 * ---------------
 * Fill chunk with voxel information.
 */
void chunk_gen_chunk(
        Chunk* chunk, 
        float chunkX, 
        float chunkZ);

void chunk_mesh_chunk(Chunk* chunk);

/* chunk_get_block
 * ---------------
 * Return the block type at the given position.
 */
u8 chunk_get_block(float x, float y, float z);

#endif
