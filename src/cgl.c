#include "cgl.h"
#include <stdio.h>
#include <stdbool.h>

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

typedef struct {
    GLFWwindow* window;
    int winW;
    int winH;
    Camera camera;
} AppData;

static AppData s_aData;

void framebuffer_size_callback(
        GLFWwindow* window, 
        int width, 
        int height) {
    glViewport(0, 0, width, height);
    s_aData.winW = width;
    s_aData.winH = height;
} 

void mouse_cursor_callback(
        GLFWwindow* window,
        double xpos,
        double ypos) {
    // Convert doubles.
    float xPos = xpos;
    float yPos = ypos;

    // So camera doesn't jolt on startup.
    static bool first = true;
    if (first) {
        input_set_cursor_x(xPos);
        input_set_cursor_y(yPos);
        first = false;
    }

    float yaw = xPos - input_get_cursor_x();
    float pitch = input_get_cursor_y() - yPos;
    cam_rotate_camera(cgl_camera(), yaw, pitch);

    input_set_cursor_x(xPos);
    input_set_cursor_y(yPos);
}

GLFWwindow* init_window() {
    // Initialise glfw.
    glfwInit();
    
    // Set the version of opengl and use core.
    glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            OPENGL_VERSION_MAJOR);

    glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR, 
            OPENGL_VERSION_MINOR);

    glfwWindowHint(
            GLFW_OPENGL_PROFILE, 
            GLFW_OPENGL_CORE_PROFILE);

    // Create the window.
    GLFWwindow* window = glfwCreateWindow(
            10, 10,
            "CGL Engine",
            NULL, NULL);

    if (!window) {
        fprintf(stderr, 
                "[ERROR] Failed to create glfw window\n");
        glfwTerminate();
        return NULL;
    }

    glfwSetInputMode(
            window, 
            GLFW_CURSOR, 
            GLFW_CURSOR_DISABLED);

    // Maximize window upon startup.
    glfwMaximizeWindow(window);
    GLFWvidmode vidMode;
    vidMode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
    s_aData.winW = vidMode.width;
    s_aData.winH = vidMode.height;

    // Set callbacks.
    glfwSetFramebufferSizeCallback(window, 
            framebuffer_size_callback);

    glfwSetCursorPosCallback(window, 
            mouse_cursor_callback);

    glfwMakeContextCurrent(window);

    return window;
}

bool cgl_init() {
    s_aData.window = init_window();

    if (!s_aData.window) {
        return false;
    }

    // Load glad.
    if (!gladLoadGLLoader(
                (GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "[ERROR] Failed to load glad\n");
        return false;
    }

    glViewport(0, 0, cgl_win_w(), cgl_win_h());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Set the window input is taken from.
    input_set_window(s_aData.window);

    // Initialise the camera.
    clmVec3 initCamPos = { 0.0f, 3.0f, 10.0f };
    cam_init_camera(
            &s_aData.camera,
            initCamPos,
            45.0f,  // fov
            0.1f,   // near
            200.0f, // far
            10.0f,   // speed
            0.1f);  // sense

    // Initialise the voxel renderer.
    voxren_init();

    return true;
}

int cgl_win_w() {
    return s_aData.winW;
}

int cgl_win_h() {
    return s_aData.winH;
}

Camera* cgl_camera() {
    return &s_aData.camera;
}

GLFWwindow* cgl_window() {
    return s_aData.window;
}
