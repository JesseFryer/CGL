#include "input.h"

typedef struct InputData {
    GLFWwindow* window;
    float cursorLastX;
    float cursorLastY;
} InputData;

static InputData s_inputData;

void input_set_window(GLFWwindow* window) {
    s_inputData.window = window;
}

bool input_is_pressed(KeyCode key) {
    return (glfwGetKey(s_inputData.window, key) == GLFW_PRESS);
}

float input_get_cursor_x() {
    return s_inputData.cursorLastX;
}

float input_get_cursor_y() {
    return s_inputData.cursorLastY;
}

void input_set_cursor_x(float x) {
    s_inputData.cursorLastX = x;
}

void input_set_cursor_y(float y) {
    s_inputData.cursorLastY = y;
}



