#ifndef CAMERA_H
#define CAMERA_H

#include "clm.h"

typedef struct Camera {
    clmMat4 proj;
    clmVec3 position;
    float   speed;
} Camera;

/* cam_init_camera
 * ---------------
 * Initialize a perspective camera.
 */
void cam_init_camera(Camera* cam,
        float fov, 
        float aspectRatio,
        clmVec3 position,
        float speed);

/* cam_move
 * --------
 * Move the camera in the direction of move.
 * move is presumed to be a unit vector.
 */
void cam_move(Camera* cam, clmVec3 move);

/* cam_view_matrix
 * ---------------
 * Store the camera view matrix in view.
 */
void cam_view_matrix(Camera* cam, clmMat4 view);

#endif
