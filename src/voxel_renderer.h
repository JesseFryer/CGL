#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

// Maximum voxels per draw call, this number seems to be most optimal
// from a small amount of testing. Going higher brings little to no
// improvement.
#define MAX_VOXELS 100 

#define VERTICES_PER_VOXEL 24
#define MAX_VERTICES       (VERTICES_PER_VOXEL * MAX_VOXELS)

#define INDICES_PER_VOXEL 36
#define MAX_INDICES       (MAX_VOXELS * INDICES_PER_VOXEL)

#include "clm.h"

typedef struct Vertex {
    float position[3]; // x, y, z
    float colour[4];   // r, g, b, a
    float normal[3];   // x, y, z
} Vertex;

/* voxren_init
 * -----------
 * Initialise buffers.
 */
void voxren_init();

/* voxren_add_vox
 * --------------
 * Submit a voxel to the batch for rendering. Size is width/height/depth.
 */
void voxren_submit_vox(
        clmVec3 position, 
        clmVec3 size,
        clmVec4 colour);

/* voxren_render_batch
 * -------------------
 * Render the current batch of voxels.
 */
void voxren_render_batch();

/* voxren_set_shader
 * -----------------
 * Set the currently used shader. If no shader is set shit won't work.
 * TODO: make a default shader be set in voxren_init.
 */
void voxren_set_shader(unsigned int shader);

/* voxren_terminate
 * ----------------
 * Free resources used by renderer.
 */
void voxren_terminate();

#endif
