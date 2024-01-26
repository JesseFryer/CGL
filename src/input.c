#include "input.h"

static GLFWwindow* s_window;

void input_set_window(GLFWwindow* window) {
    s_window = window;
}

bool input_is_pressed(KeyCode key) {
    return (glfwGetKey(s_window, key) == GLFW_PRESS);
}

