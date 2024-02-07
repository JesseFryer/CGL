#include "cgl.h"
#include "perlin.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main() {
    AppData* app = cgl_init();
    
    // Directional light.
    clmVec3 lightPos = { 250.0f, 200.0f, 250.0f };

    // Testing out the sprite atlas thingy.
    SpriteAtlas atlas;
    VoxelTex voxTex;
    tex_init_atlas(&atlas, 256, 256, 16, 16);
    tex_create_voxel_tex(&voxTex, &atlas, 240, 242, 243);
    
    double lastTime = glfwGetTime();
    float reportFrameTimer = 0.0f;
    bool running = true;
    while(running) {
        // Delta time.
        double currTime = glfwGetTime();
        double deltaTime = currTime - lastTime;
        float timeStep = deltaTime;
        lastTime = currTime;
        reportFrameTimer += timeStep;

        // Report fps.
        if (reportFrameTimer >= 1.0f) {
            printf("FPS: %.2f\n", 1.0f / timeStep);
            reportFrameTimer = 0.0f;
        }

        // Events and program exit.
        glfwPollEvents();
        if (glfwWindowShouldClose(app->window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

        cgl_move_camera(timeStep);

        cgl_update_view();

        // Set uniforms.
        shader_set_uniform_vec3(
                app->voxelShader, 
                "vLightPos", 
                lightPos);
        

        // Render.
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

        clmVec4 white  = { 1.0f, 1.0f, 1.0f, 1.0f };
        clmVec3 voxSize = { 1.0f, 1.0f, 1.0f };
        clmVec3 voxPos  = { 0.0f, 0.0f, 0.0f };

        // Render a bunch using noise
        float chunkW = 100.0f;
        shader_use(app->voxelShader);
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
        shader_use(app->lightShader);
        clmVec3 lightSize = { 20.0f, 20.0f, 20.0f };
        clmVec3 lightCol = { 1.0f, 1.0f, 1.0f };
        voxren_submit_vox(
                lightPos, 
                lightSize, 
                white, 
                &voxTex);
        voxren_render_batch();

        glfwSwapBuffers(app->window);
    }

    voxren_terminate();
    glfwTerminate();

    return 0;
}
