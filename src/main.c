#include "cgl.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main() {
    cgl_init();

    GLFWwindow* window = cgl_window();

    // Create shaders.
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
    clmVec3 lightPos = { 0.0f, 10.0f, 0.0f };
    float moveRadius = 12.0f;
    float lightSpeed = 0.5f;
    float centre = 0.0f;

    // Testing out the sprite atlas thingy.
    SpriteAtlas atlas;
    VoxelTex voxTex;
    tex_init_atlas(&atlas, 256, 256, 16, 16);
    tex_create_voxel_tex(&voxTex, &atlas, 240, 242, 243);

    // Test texture loading.
    unsigned int wallTex = 
        tex_load_texture("wall.png");

    unsigned int blockAtlas = 
        tex_load_texture("block_tex_atlas.png");

    unsigned int mcAtlas = 
        tex_load_texture("minecraft_atlas.png");
    
    glBindTexture(GL_TEXTURE_2D, mcAtlas);

    double lastTime = glfwGetTime();
    float reportFrameTimer = 0.0f;
    int frameCount = 0;
    bool running = true;
    while(running) {
        // Delta time.
        double currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;
        reportFrameTimer += deltaTime;
        frameCount++;

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

        // Move the light about
        lightPos[0] = centre + moveRadius * cos(currTime);
        lightPos[2] = centre + moveRadius * sin(currTime);

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
        cam_move(cgl_camera(), camMove, deltaTime);

        // Update our view and projection transforms.
        cam_view_matrix(
                cgl_camera(), 
                view);

        cam_proj_matrix(
                cgl_camera(),
                (float) cgl_win_w() / (float) cgl_win_h(),
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

        clmVec4 white  = { 1.0f, 1.0f, 1.0f, 1.0f };
        clmVec3 voxSize = { 5.0f, 5.0f, 5.0f };
        clmVec3 voxPos  = { 0.0f, 0.0f, 0.0f };

        // Grass block.
        shader_use(shader);
        voxren_submit_vox(
                voxPos, 
                voxSize, 
                white,
                &voxTex);
        voxren_render_batch();

        // Rotating light.
        shader_use(lightShader);
        clmVec3 lightSize = { 2.0f, 2.0f, 2.0f };
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
