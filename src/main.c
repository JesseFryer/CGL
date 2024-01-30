#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "input.h"
#include "shader.h"
#include "vao.h"
#include "vertex.h"
#include "clm.h"
#include "camera.h"
#include "object.h"

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
    GLFWvidmode vidMode = 
        *glfwGetVideoMode(glfwGetPrimaryMonitor());
    SCR_W = vidMode.width;
    SCR_H = vidMode.height;
    CURS_LAST_X = SCR_W / 2;
    CURS_LAST_Y = SCR_H / 2;

    return window;
}

int main() {
    // Create the window.
    GLFWwindow* window = init_window(
            "Learning OpenGL", 
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
    glfwSetFramebufferSizeCallback(window, 
            framebuffer_size_callback);
    glfwSetCursorPosCallback(window, 
            mouse_cursor_callback);

    // Create some data.
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    // Defeats the prupose lazy but fix later.
    unsigned int indices[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        35
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(vertices), 
            vertices,
            GL_STATIC_DRAW);

    // Stores layout and indices.
    VAO vao;
    vao_init(&vao);

    // Give ebo indices.
    vao_ebo_data(
            &vao,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW,
            36);

    // Specify layout.
    vao_vertex_attrib(
            &vao,
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*) offsetof(Vertex, position));

    unsigned int shader = shader_create(
            "../src/shaders/vshader.glsl",
            "../src/shaders/fshader.glsl");

    glUseProgram(shader);
    vao_bind(&vao);

    // vp matrices.
    clmMat4 view;
    clmMat4 proj;

    // For our camera.
    clmVec3 camMove = { 0.0f, 0.0f, 0.0f };
    clmVec3 initCamPos = { 0.0f, 0.0f, 3.0f };
    cam_init_camera(&camera,
            initCamPos,
            45.0f,   // fov
            0.1f,    // near
            100.0f,  // far
            0.002f,  // speed
            0.04f);  // sense

    // Lets make some cubes.
    Object cube1;
    Object cube2;
    Object cube3;

    obj_set_vbo(&cube1, vbo);
    obj_set_vao(&cube1, &vao);

    obj_set_vbo(&cube2, vbo);
    obj_set_vao(&cube2, &vao);

    obj_set_vbo(&cube3, vbo);
    obj_set_vao(&cube3, &vao);

    clmVec3 cubePos1 = { -2.0f, 3.0f, 7.0f };
    clmVec3 cubePos2 = { 1.0f, 0.0f, 2.0f };
    clmVec3 cubePos3 = { 2.0f, -6.0f, 4.0f };
    obj_set_pos(&cube1, cubePos1);
    obj_set_pos(&cube2, cubePos2);
    obj_set_pos(&cube3, cubePos3);

    bool running = true;
    while(running) {
        // Events and program exit.
        glfwPollEvents();
        if (glfwWindowShouldClose(window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

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

        unsigned int viewLoc = glGetUniformLocation(
                shader, "view");
        glUniformMatrix4fv(viewLoc,
                1, GL_FALSE, view);

        unsigned int projLoc = glGetUniformLocation(
                shader, "proj");
        glUniformMatrix4fv(projLoc,
                1, GL_FALSE, proj);

        // Draw.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        obj_render(&cube1, shader);
        obj_render(&cube2, shader);
        obj_render(&cube3, shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}
