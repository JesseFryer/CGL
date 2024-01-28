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

static int SCR_W = 1600;
static int SCR_H = 900;

void framebuffer_size_callback(
        GLFWwindow* window, 
        int width, 
        int height) {
    glViewport(0, 0, width, height);
    SCR_W = width;
    SCR_H = height;
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

    //glfwSetInputMode(window, GLFW_CURSOR, 
    //        GLFW_CURSOR_DISABLED);

    // Maximize window and set SCR_W, SCR_H
    glfwMaximizeWindow(window);
    GLFWvidmode vidMode = 
        *glfwGetVideoMode(glfwGetPrimaryMonitor());
    SCR_W = vidMode.width;
    SCR_H = vidMode.height;

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
    glfwSetFramebufferSizeCallback(
            window, 
            framebuffer_size_callback);

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

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
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
            GL_STATIC_DRAW);

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

    // Camera. View and projection matrices are supplied by
    // the camera.
    Camera camera;
    clmVec3 initCamPos = { 0.0f, 0.0f, 3.0f };
    cam_init_camera(&camera,
            initCamPos,
            45.0f,   // fov
            0.1f,    // near
            100.0f,  // far
            0.005f); // speed

    // For our camera.
    clmVec3 camMove = { 0.0f, 0.0f, 0.0f };

    // For our cube.
    clmVec3 rotate = { 1.0f, 1.0f, 1.0f };
    clm_v3_normalize(rotate);

    // mvp matrices.
    clmMat4 model;
    clmMat4 view;
    clmMat4 proj;

    bool running = true;
    while(running) {
        // Events and program exit.
        glfwPollEvents();
        if (glfwWindowShouldClose(window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

        // Rotating our cube here.
        clm_mat4_identity(model);
        clm_mat4_rotate(model, 
                5.0f * ((float) glfwGetTime()), 
                rotate);

        // Move camera.
        camMove[0] = 0.0f;
        camMove[1] = 0.0f;
        camMove[2] = 0.0f;
        if (input_is_pressed(K_W)) {
            camMove[2] -= 1.0f;
        } 
        if (input_is_pressed(K_S)) {
            camMove[2] += 1.0f;
        } 
        if (input_is_pressed(K_D)) {
            camMove[0] += 1.0f;
        } 
        if (input_is_pressed(K_A)) {
            camMove[0] -= 1.0f;
        } 
        if (input_is_pressed(K_SPACE)) {
            camMove[1] += 1.0f;
        } 
        if (input_is_pressed(K_LSHIFT)) {
            camMove[1] -= 1.0f;
        } 
        cam_move(&camera, camMove);

        // Update our view and projection transforms.
        cam_view_matrix(&camera, view);
        cam_proj_matrix(&camera,
                (float) SCR_W / (float) SCR_H,
                proj);

        // Supply mvp matrices to shader.
        unsigned int modelLoc = glGetUniformLocation(
                shader, "model");
        glUniformMatrix4fv(modelLoc,
                1, GL_FALSE, model);

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
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}
