#ifndef CAMERA_H
#define CAMERA_H

#include "clm.h"

typedef struct Camera {
    // View matrix things.
    clmVec3 position;
    clmVec3 up;

    // Perspective projection things.
    float   fov;
    float   near;
    float   far;

    // Camera direction.
    float   yaw;
    float   pitch;

    // Camera move speed.
    float   speed;
    float   sense;
} Camera;

/* cam_init_camera
 * ---------------
 * Initialize a perspective camera. Is useful for producing
 * both the projection and view transforms.
 */
void cam_init_camera(Camera* cam,
        clmVec3 position,
        float fov, 
        float near,
        float far,
        float speed,
        float sense);

/* cam_move
 * --------
 * Move the camera in the direction of move.
 * move is presumed to be a unit vector.
 */
void cam_move(Camera* cam, clmVec3 move);

/* cam_view_matrix
 * ---------------
 * Store the camera view matrix in view.
 * The view matrix is a lookat matrix.
 */
void cam_view_matrix(Camera* cam, clmMat4 view);

/* cam_proj_matrix
 * ---------------
 * Store the camera projection matrix in proj. 
 * Recalculate this with the new aspect ratio
 * whenever the window is resized.
 * The projection matrix is a perspective projection matrix.
 */
void cam_proj_matrix(Camera* cam, 
        float aspectRatio,
        clmMat4 proj);

/* cam_rotate_camera
 * -----------------
 * Rotate the camera by an offset.
 */
void cam_rotate_camera(Camera* cam, float yaw, float pitch);

#endif
