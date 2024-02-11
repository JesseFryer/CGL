#include "cgl.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

static AppData s_aData;

void framebuffer_size_callback(
        GLFWwindow* window, 
        int width, 
        int height) {
    // Update viewport.
    s_aData.winW = width;
    s_aData.winH = height;
    glViewport(0, 0, width, height);

    // Recalculate projection matrix.
    cam_proj_matrix(
            &s_aData.camera,
            (float) width /
            (float) height,
            s_aData.proj);

    // Update projection uniforms.
    shader_set_uniform_mat4(
            s_aData.voxelShader, 
            "proj", 
            s_aData.proj);
    shader_set_uniform_mat4(
            s_aData.lightShader, 
            "proj", 
            s_aData.proj);
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
    cam_rotate_camera(&s_aData.camera, yaw, pitch);

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

    // Disable cursor.
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

    // Context.
    glfwMakeContextCurrent(window);

    // Vsync.
    glfwSwapInterval(1);

    return window;
}

void load_textures() {
    // The order in which textures are loaded determines
    // their corresponding texture slot.
    tex_load_texture("minecraft_atlas.png"); // slot 0
    tex_load_texture("font.png");            // slot 1
}

AppData* cgl_init() {
    s_aData.window = init_window();

    if (!s_aData.window) {
        return NULL;
    }

    // Load glad.
    if (!gladLoadGLLoader(
                (GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "[ERROR] Failed to load glad\n");
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Textures.
    load_textures();

    // Set the window input is taken from.
    input_set_window(s_aData.window);

    // Initialise the camera.
    clmVec3 initCamPos = { 0.0f, 132.0f, 0.0f };
    cam_init_camera(
            &s_aData.camera,
            initCamPos,
            45.0f,   // fov
            0.1f,    // near
            1000.0f, // far
            50.0f,   // speed
            0.1f);   // sense
  
    // Initialise the voxel renderer.
    voxren_init();

    // Create shaders.
    s_aData.voxelShader = shader_create(
            "vshader.glsl",
            "fshader.glsl");

    s_aData.lightShader = shader_create(
            "vlightshader.glsl",
            "flightshader.glsl");

    // Set viewport, initialise projection matrix
    // and set the projection uniforms in shader.
    framebuffer_size_callback(
            s_aData.window, 
            s_aData.winW, 
            s_aData.winH);

    return &s_aData;
}

void cgl_update_view() {
    // Recalculate view matrix.
    cam_view_matrix(
            &s_aData.camera, 
            s_aData.view);

    // Update uniforms.
    shader_set_uniform_mat4(
            s_aData.voxelShader, 
            "view", 
            s_aData.view);

    shader_set_uniform_mat4(
            s_aData.lightShader,
            "view", 
            s_aData.view);

    shader_set_uniform_vec3(
            s_aData.voxelShader, 
            "vCamPos", 
            s_aData.camera.position);

}

void cgl_move_camera(float timeStep) {
    // Move the camera about.
    clmVec3 camMove = { 0.0f };
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
    cam_move(&s_aData.camera, camMove, timeStep);
}

void generate_chunks() {
    // Calculate player chunk.
    float playerChunkX = floorf(
            s_aData.camera.position[0] / CHUNK_W);
    float playerChunkZ = floorf(
            s_aData.camera.position[2] / CHUNK_D);

    // Check if chunk is in render region.
    int chunkIndex = 0;
    for (int x = 0; x < 4; x++) {
        for (int z = 0; z < 4; z++) {
            Chunk* chunk = &s_aData.chunks[chunkIndex++];
            float chunkX = playerChunkX + x;
            float chunkZ = playerChunkZ + z;
            if (chunk->x != chunkX || 
                    chunk->z != chunkZ) {
                chunk_gen_chunk(chunk, chunkX, chunkZ);
            }
        }
    }
}

void cgl_run() {
    for (int i = 0; i < 16; i++) {
        s_aData.chunks[i].x = -1.0f;
        s_aData.chunks[i].z = -1.0f;
    }
    generate_chunks();

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
        if (glfwWindowShouldClose(s_aData.window) || 
                input_is_pressed(K_ESC)) {
            running = false;
        }

        generate_chunks();

        cgl_move_camera(timeStep);

        cgl_update_view();

        // Set uniforms.
        shader_set_uniform_vec3(
                s_aData.voxelShader, 
                "vLightPos", 
                lightPos);

        // Render.
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

        clmVec4 white  = { 1.0f, 1.0f, 1.0f, 1.0f };
        clmVec3 voxSize = { 1.0f, 1.0f, 1.0f };
        clmVec3 voxPos  = { 0.0f, 0.0f, 0.0f };

        shader_use(s_aData.voxelShader);

        for (int i = 0; i < 16; i++) {
            chunk_render(&s_aData.chunks[i]);
        }

        // Rotating light.
        shader_use(s_aData.lightShader);
        clmVec3 lightSize = { 20.0f, 20.0f, 20.0f };
        clmVec3 lightCol = { 1.0f, 1.0f, 1.0f };
        voxren_submit_vox(
                lightPos, 
                lightSize, 
                white, 
                &voxTex);
        voxren_render_batch();

        glfwSwapBuffers(s_aData.window);
    }
}

void cgl_cleanup() {
    voxren_terminate();
    glfwTerminate();
}
