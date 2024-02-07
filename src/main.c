#include "cgl.h"
#include "perlin.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>

#define FRAME_CAP  144.0
#define FRAME_TIME (1.0 / FRAME_CAP)

int main() {
    cgl_init();

    GLFWwindow* window = cgl_window();

    unsigned int shader = shader_create(
            "vshader.glsl",
            "fshader.glsl");

    unsigned int lightShader = shader_create(
            "vlightshader.glsl",
            "flightshader.glsl");

    // vp matrices.
    clmMat4 view;
    clmMat4 proj;

    // Camera move directions.
    clmVec3 camMove = { 0.0f, 0.0f, 0.0f };
    
    // Directional light.
    clmVec3 lightPos = { 250.0f, 200.0f, 250.0f };

    // Testing out the sprite atlas thingy.
    SpriteAtlas atlas;
    VoxelTex voxTex;
    tex_init_atlas(&atlas, 256, 256, 16, 16);
    tex_create_voxel_tex(&voxTex, &atlas, 240, 242, 243);
    
    double lastTime = glfwGetTime() - FRAME_TIME;
    float reportFrameTimer = 0.0f;
    int frameCount = 0;
    bool running = true;
    while(running) {
        // Delta time.
        double currTime = glfwGetTime();
        double deltaTime = currTime - lastTime;
        float timeStep = deltaTime;
        lastTime = currTime;
        reportFrameTimer += timeStep;
        frameCount++;

        // Cap fps.
        if (deltaTime < FRAME_TIME) {
            unsigned int dur = 1000.0 * (FRAME_TIME - deltaTime);
            Sleep(dur);
        }

        // Report fps.
        if (reportFrameTimer >= 1.0f) {
            printf("FPS: %d\n", frameCount);
            reportFrameTimer = 0.0f;
            frameCount = 0;
        }

        // Events and program exit.
        glfwPollEvents();
        if (glfwWindowShouldClose(window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

        // Move the camera about.
        camMove[0] = 0.0f; // forward/back 1/-1
        camMove[1] = 0.0f; // right/left   1/-1
        camMove[2] = 0.0f; // up/down      1/-1
        if (input_is_pressed(K_W)) {
            camMove[0] += 1.0f;
        } 
        if (input_is_pressed(K_S)) {
            camMove[0] -= 1.0f;
        } 
        if (input_is_pressed(K_D)) {
            camMove[1] += 1.0f;
        } 
        if (input_is_pressed(K_A)) {
            camMove[1] -= 1.0f;
        } 
        if (input_is_pressed(K_SPACE)) {
            camMove[2] += 1.0f;
        } 
        if (input_is_pressed(K_LSHIFT)) {
            camMove[2] -= 1.0f;
        } 

        cam_move(cgl_camera(), camMove, timeStep);

        // Update our view and projection transforms.
        cam_view_matrix(
                cgl_camera(), 
                view);

        cam_proj_matrix(
                cgl_camera(),
                (float) cgl_win_w() /
                (float) cgl_win_h(),
                proj);

        // Set uniforms.
        shader_set_uniform_vec3(
                shader, 
                "vLightPos", 
                lightPos);
        
        shader_set_uniform_vec3(
                shader, 
                "vCamPos", 
                cgl_camera()->position);

        shader_set_uniform_mat4(
                shader, 
                "view", 
                view);

        shader_set_uniform_mat4(
                shader, 
                "proj", 
                proj);

        shader_set_uniform_mat4(
                lightShader, 
                "view", 
                view);

        shader_set_uniform_mat4(
                lightShader, 
                "proj", 
                proj);

        // Render.
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

        clmVec4 white  = { 1.0f, 1.0f, 1.0f, 1.0f };
        clmVec3 voxSize = { 1.0f, 1.0f, 1.0f };
        clmVec3 voxPos  = { 0.0f, 0.0f, 0.0f };

        // Render a bunch using noise
        float chunkW = 100.0f;
        shader_use(shader);
        for (float x = 0.0f; x < chunkW; x++) {
            for (float z = 0.0f; z < chunkW; z++) {
                float y = floorf(5.0f * perlin2d(x, z, 0.1, 4));
                voxPos[0] = x;
                voxPos[1] = y;
                voxPos[2] = z;
                voxren_submit_vox(
                        voxPos, 
                        voxSize, 
                        white,
                        &voxTex);
            }
        }
        voxren_render_batch();

        // Rotating light.
        shader_use(lightShader);
        clmVec3 lightSize = { 20.0f, 20.0f, 20.0f };
        clmVec3 lightCol = { 1.0f, 1.0f, 1.0f };
        voxren_submit_vox(
                lightPos, 
                lightSize, 
                white, 
                &voxTex);
        voxren_render_batch();

        glfwSwapBuffers(window);
    }

    voxren_terminate();
    glfwTerminate();

    return 0;
}
