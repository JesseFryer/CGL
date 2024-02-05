#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "input.h"
#include "shader.h"
#include "vao.h"
#include "clm.h"
#include "camera.h"
#include "voxel_renderer.h"
#include "texture.h"
#include <math.h>

#define CHUNK_SIZE 8

static int SCR_W = 1600;
static int SCR_H = 900;

static Camera camera;

void framebuffer_size_callback(
        GLFWwindow* window, 
        int width, 
        int height) {
    glViewport(0, 0, width, height);
    SCR_W = width;
    SCR_H = height;
} 

void mouse_cursor_callback(
        GLFWwindow* window,
        double xpos,
        double ypos) {
    static bool first = true;

    float xPos = xpos;
    float yPos = ypos;

    if (first) {
        input_set_cursor_x(xPos);
        input_set_cursor_y(yPos);
        first = false;
    }

    float yaw = xPos - input_get_cursor_x();
    float pitch = input_get_cursor_y() - yPos;

    cam_rotate_camera(&camera, yaw, pitch);

    input_set_cursor_x(xPos);
    input_set_cursor_y(yPos);
}

GLFWwindow* init_window(
        const char* title, 
        int width, 
        int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, 
            GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
            width, height,
            title,
            NULL, NULL);

    if (!window) {
        printf("Unable to make a glfw window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, 
            GLFW_CURSOR_DISABLED);

    // Maximize window and set SCR_W, SCR_H
    glfwMaximizeWindow(window);
    GLFWvidmode vidMode;
    vidMode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
    SCR_W = vidMode.width;
    SCR_H = vidMode.height;

    return window;
}

int main() {
    // Create the window.
    GLFWwindow* window = init_window(
            "Voxoff Engine", 
            SCR_W, 
            SCR_H);

    // Set the window input is taken from.
    input_set_window(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not load glad stuff\n");
        return -1;
    }

    glViewport(0, 0, SCR_W, SCR_H);
    glEnable(GL_DEPTH_TEST);



    // Set callbacks.
    glfwSetFramebufferSizeCallback(window, 
            framebuffer_size_callback);
    glfwSetCursorPosCallback(window, 
            mouse_cursor_callback);

    // Create shaders.
    unsigned int shader = shader_create(
            "vshader.glsl",
            "fshader.glsl");

    unsigned int lightShader = shader_create(
            "vlightshader.glsl",
            "flightshader.glsl");

    // Initialise voxel renderer.
    voxren_init();

    // vp matrices.
    clmMat4 view;
    clmMat4 proj;

    // Initialise camera.
    clmVec3 camMove = { 0.0f, 0.0f, 0.0f };
    clmVec3 initCamPos = { 0.0f, 10.0f, 0.0f };
    cam_init_camera(&camera,
            initCamPos,
            45.0f,  // fov
            0.1f,   // near
            500.0f, // far
            20.0f,   // speed
            0.1f);  // sense
    
    // Directional light.
    clmVec3 lightPos = { 0.0f, 8.0f, 0.0f };
    float moveRadius = 8.0f;
    float lightSpeed = 0.5f;
    float centre = 0.0f;
    
    bool running = true;
    double lastTime = glfwGetTime();
    float reportFrameTimer = 0.0f;

    // Testing out the sprite atlas thingy.
    SpriteAtlas atlas;
    VoxelTex voxTex;
    tex_init_atlas(&atlas, 256, 256, 16, 16);
    tex_create_voxel_tex(&voxTex, &atlas, 229, 229, 228);
    printf("top side bottom\n");
    clm_v2_print(voxTex.top);
    clm_v2_print(voxTex.side);
    clm_v2_print(voxTex.bottom);
    printf("\nnormlaized dimensions (x,y): (%.4f, %.4f)\n", 
            voxTex.normSpriteDim[0],
            voxTex.normSpriteDim[1]);

    // Test texture loading.
    unsigned int wallTex = tex_load_texture("wall.png");
    unsigned int blockAtlas = tex_load_texture("block_tex_atlas.png");
    unsigned int mcAtlas = tex_load_texture("minecraft_atlas.png");
    glBindTexture(GL_TEXTURE_2D, mcAtlas);

    int chunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {0};
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk[x][y][z] = 1;
            }
        }
    }

    while(running) {
        // Delta time.
        double currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;
        reportFrameTimer += deltaTime;

        if (reportFrameTimer >= 1.0f) {
            printf("frame time (ms): %.4f\n", deltaTime * 1000.0f);
            reportFrameTimer = 0.0f;
        }

        // Events and program exit.
        glfwPollEvents();
        if (glfwWindowShouldClose(window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

        lightPos[0] = centre + moveRadius * cos(currTime);
        lightPos[2] = centre + moveRadius * sin(currTime);

        // Move camera.
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
        cam_move(&camera, camMove, deltaTime);

        // Update our view and projection transforms.
        cam_view_matrix(&camera, view);
        cam_proj_matrix(&camera,
                (float) SCR_W / (float) SCR_H,
                proj);

        // Set uniforms.
        shader_set_uniform_vec3(shader, "vLightPos", lightPos);
        shader_set_uniform_vec3(shader, "vCamPos", camera.position);
        shader_set_uniform_mat4(shader, "view", view);
        shader_set_uniform_mat4(shader, "proj", proj);
        shader_set_uniform_mat4(lightShader, "view", view);
        shader_set_uniform_mat4(lightShader, "proj", proj);

        // Render.
        glClearColor(0.1f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        clmVec4 white  = { 1.0f, 1.0f, 1.0f, 1.0f };

        clmVec3 voxSize = { 1.0f, 1.0f, 1.0f };
        clmVec3 voxPos  = { 0.0f, 0.0f, 0.0f };

        shader_use(shader);
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    if (chunk[x][y][z]) {
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
            }
        }
        voxren_render_batch();

        shader_use(lightShader);
        clmVec3 lightSize = { 2.0f, 2.0f, 2.0f };
        clmVec3 lightCol = { 1.0f, 1.0f, 1.0f };
        voxren_submit_vox(lightPos, lightSize, white, &voxTex);
        voxren_render_batch();

        glfwSwapBuffers(window);
    }

    voxren_terminate();
    glfwTerminate();
    return 0;

}
