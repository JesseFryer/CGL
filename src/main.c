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
#include <math.h>

static int SCR_W = 1600;
static int SCR_H = 900;

static float CURS_LAST_X;
static float CURS_LAST_Y;

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
    if (first) {
        CURS_LAST_X = xpos;
        CURS_LAST_Y = ypos;
        first = false;
    }

    float yaw = xpos - CURS_LAST_X;
    float pitch = CURS_LAST_Y - ypos;

    cam_rotate_camera(&camera, yaw, pitch);

    CURS_LAST_X = xpos;
    CURS_LAST_Y = ypos;
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
    CURS_LAST_X = SCR_W / 2;
    CURS_LAST_Y = SCR_H / 2;

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

    // Create shader.
    unsigned int shader = shader_create(
            "../src/shaders/vshader.glsl",
            "../src/shaders/fshader.glsl");

    unsigned int lightShader = shader_create(
            "../src/shaders/vlightshader.glsl",
            "../src/shaders/flightshader.glsl");

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
            0.005f,  // speed
            0.1f);  // sense
    
    // Directional light.
    clmVec3 lightPos = { 0.0f, 8.0f, 0.0f };
    float moveRadius = 12.0f;
    float lightSpeed = 0.5f;
    float centre = 0.0f;
    
    bool running = true;
    double lastTime = glfwGetTime();
    float reportFrameTimer = 0.0f;

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
        cam_move(&camera, camMove);

        // Update our view and projection transforms.
        cam_view_matrix(&camera, view);
        cam_proj_matrix(&camera,
                (float) SCR_W / (float) SCR_H,
                proj);

        glUseProgram(shader);
        unsigned int viewLoc = glGetUniformLocation(
                shader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);

        unsigned int projLoc = glGetUniformLocation(
                shader, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);

        unsigned int lightLoc = glGetUniformLocation(
                shader, "vLightPos");
        glUniform3fv(lightLoc,
                1, lightPos);

        unsigned int camPosLoc = glGetUniformLocation(
                shader, "vCamPos");
        glUniform3fv(camPosLoc,
                1, camera.position);

        glUseProgram(lightShader);
        viewLoc = glGetUniformLocation(lightShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        projLoc = glGetUniformLocation(lightShader, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);

        // Render.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        clmVec3 voxPos = { 0.0f, 0.0f, 0.0f };

        clmVec4 voxCol = { 0.2f, 0.3f, 1.0f, 1.0f };
        clmVec4 lightCol = { 1.0f, 1.0f, 1.0f, 1.0f };

        clmVec3 voxSize = { 50.0f, 0.5f, 50.0f };
        clmVec3 lightSize = { 0.25f, 0.25f, 0.25f };

        voxren_set_shader(shader);
        voxren_submit_vox(voxPos, voxSize, voxCol);
        voxPos[1] = 10.0f;
        voxSize[0] = 5.0f;
        voxSize[1] = 5.0f;
        voxSize[2] = 5.0f;
        voxCol[0] = 0.0f;
        voxCol[1] = 1.0f;
        voxCol[2] = 0.0f;
        voxren_submit_vox(voxPos, voxSize, voxCol);
        voxren_render_batch();

        voxren_set_shader(lightShader);
        voxren_submit_vox(lightPos, lightSize, lightCol);
        voxren_render_batch();

        glfwSwapBuffers(window);
    }

    voxren_terminate();
    glfwTerminate();
    return 0;

}
