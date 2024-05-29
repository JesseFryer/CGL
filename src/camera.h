#ifndef CAMERA_H
#define CAMERA_H

#include "clm.h"

typedef struct {
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
 * Initialise a perspective camera. 
 * Is useful for producing both the projection and 
 * view transforms.
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
 * clmVec3 move represents:
 *    { forward/back, up/down, right/left }
 * and can take on the values:
 *    { 1/0/-1, 1/0/-1, 1/0/-1 }
 * 
 * setting move[0] = 1 for example will move the camera
 * forward according to its direction and speed.
 */
void cam_move(
        Camera* cam, 
        clmVec3 move, 
        float deltaTime);

/* cam_move_by_value
 * -----------------
 * Same as cam_move but direction is fixed to x y z.
 * Better for orthographic camera.
 */
void cam_move_simple(
        Camera* cam, 
        clmVec3 move, 
        float deltaTime);

/* cam_view_matrix
 * ---------------
 * Calculate and store the camera view matrix in view.
 * The view matrix is a lookat matrix.
 */
void cam_view_matrix(Camera* cam, clmMat4 view);

/* cam_proj_matrix
 * ---------------
 * Calculate and store the camera projection matrix in 
 * proj. 
 *
 * This will need to be recalculated with the new aspect 
 * ratio if the window is resized.
 */
void cam_proj_matrix(
        Camera* cam, 
        float aspectRatio,
        clmMat4 proj);

/* cam_rotate_camera
 * -----------------
 * Rotate the camera by yaw and pitch.
 */
void cam_rotate_camera(
        Camera* cam, 
        float yaw, 
        float pitch);

#endif
