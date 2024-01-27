#include "camera.h"

void cam_init_camera(Camera* cam, 
        float fov, 
        float aspectRatio,
        clmVec3 position,
        float speed) {
    clm_mat4_perspective(cam->proj, 
            fov, 
            aspectRatio,
            0.1f,
            100.0f);
    cam->speed = speed;
    cam->position[0] = position[0];
    cam->position[1] = position[1];
    cam->position[2] = position[2];
}

void cam_move(Camera* cam, clmVec3 move) {
    float speed = cam->speed;
    cam->position[0] += speed * move[0];
    cam->position[1] += speed * move[1];
    cam->position[2] += speed * move[2];
}

void cam_view_matrix(Camera* cam, clmMat4 view) {
    clm_mat4_identity(view);
    clm_mat4_translate(view, cam->position);
}
