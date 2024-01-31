#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#define VERTICES_PER_VOXEL 24
#define MAX_VOXELS         1000
#define MAX_VERICES        (VERTICES_PER_VOXEL * MAX_VOXELS)

#define INDICES_PER_VOXEL 36
#define MAX_INDICES       (MAX_VOXELS * INDICES_PER_VOXEL)

/* voxren_init
 * -----------
 * Initialise buffers.
 */
void voxren_init();

#endif
