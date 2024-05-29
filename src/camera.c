#include "camera.h"
#include <math.h>

void front_vector(Camera* cam, clmVec3 front) {
    // determine front vector from euler angles.
    float yaw = clm_radians(cam->yaw);
    float pitch = clm_radians(cam->pitch);
    front[0] = cosf(yaw) * cosf(pitch);
    front[1] = sinf(pitch);
    front[2] = sinf(yaw) * cosf(pitch);
    clm_v3_normalize(front);
}

void cam_init_camera(Camera* cam, 
        clmVec3 position,
        float fov, 
        float near,
        float far,
        float speed,
        float sense) {
    cam->position[0] = position[0];
    cam->position[1] = position[1];
    cam->position[2] = position[2];

    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;

    cam->fov  = fov;
    cam->near = near;
    cam->far  = far;

    cam->yaw = -90.0f;
    cam->pitch = 0.0f;

    cam->speed = speed;
    cam->sense = sense;
}

void cam_move(Camera* cam, clmVec3 move, float deltaTime) {
    // Grammmm SHmitttt. 
    // TODO: This is done in clm_mat4_lookat should probs not
    //       duplicate this code kind of waste of time.
    //
    clmVec3 front;
    clmVec3 side;
    clmVec3 up = { cam->up[0], cam->up[1], cam->up[2] };

    front_vector(cam, front);
    // These two lines will make it like minecraft.
    front[1] = 0.0f; 
    clm_v3_normalize(front);

    clm_v3_cross_product(front, cam->up, side);
    clm_v3_normalize(side);

    // move will contain values 1/-1/0, so determines which direction.
    float frontScalar = cam->speed * deltaTime * move[0];
    float sideScalar  = cam->speed * deltaTime * move[1];
    float upScalar    = cam->speed * deltaTime * move[2];

    clm_v3_scalar_multiply(frontScalar, front);
    clm_v3_scalar_multiply(sideScalar, side);
    clm_v3_scalar_multiply(upScalar, up);

    clm_v3_add(cam->position, front, cam->position); 
    clm_v3_add(cam->position, side, cam->position); 
    clm_v3_add(cam->position, up, cam->position); 
}                                                    

void cam_move_simple(
        Camera* cam, 
        clmVec3 move, 
        float deltaTime) {
    clm_v3_scalar_multiply(deltaTime, move);
    clm_v3_scalar_multiply(cam->speed, move);
    clm_v3_add(cam->position, move, cam->position);
}
                                                     
void cam_view_matrix(Camera* cam, clmMat4 view) {
    clmVec3 front;
    front_vector(cam, front);
    clmVec3 target;
    clm_v3_add(cam->position, front, target);
    clm_mat4_lookat(view,
            cam->position,
            target,
            cam->up);
}

void cam_proj_matrix(Camera* cam, 
        float aspectRatio,
        clmMat4 proj) {
    // Store the perspective projection in proj.
    clm_mat4_perspective(proj,
            cam->fov,
            aspectRatio,
            cam->near,
            cam->far);
}

void cam_rotate_camera(Camera* cam, float yaw, float pitch) {
    cam->yaw += cam->sense * yaw;
    cam->pitch += cam->sense * pitch;
    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    }
    if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }
}
