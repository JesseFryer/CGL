#include "camera.h"

void cam_init_camera(Camera* cam, 
        float fov, 
        clmVec3 position,
        float speed) {
    cam->speed = speed;
    cam->position[0] = position[0];
    cam->position[1] = position[1];
    cam->position[2] = position[2];
    cam->up[0] = 0.0f;
    cam->up[1] = 1.0f;
    cam->up[2] = 0.0f;
    cam->front[0] = 0.0f;
    cam->front[1] = 0.0f;
    cam->front[2] = -1.0f;
    cam->yaw = -90.0f;
    cam->pitch = 0.0f;
}

void cam_move(Camera* cam, clmVec3 move) {
    float speed = cam->speed;
    cam->position[0] += speed * move[0];
    cam->position[1] += speed * move[1];
    cam->position[2] += speed * move[2];
}

void cam_view_matrix(Camera* cam, clmMat4 view) {
    clmVec3 target = { 
        cam->position[0] + cam->front[0],
        cam->position[1] + cam->front[1],
        cam->position[2] + cam->front[2]
    };
    clm_mat4_lookat(view,
            cam->position,
            target,
            cam->up);
}
