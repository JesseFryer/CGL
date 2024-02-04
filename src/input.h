/* input.h
 *
 * Retrieve input state. A wrapper around glfw.
 *
 */

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef int KeyCode;

#define K_A  GLFW_KEY_A
#define K_B  GLFW_KEY_B
#define K_C  GLFW_KEY_C
#define K_D  GLFW_KEY_D
#define K_E  GLFW_KEY_E
#define K_F  GLFW_KEY_F
#define K_G  GLFW_KEY_G
#define K_H  GLFW_KEY_H
#define K_I  GLFW_KEY_I
#define K_J  GLFW_KEY_J
#define K_K  GLFW_KEY_K
#define K_L  GLFW_KEY_L
#define K_M  GLFW_KEY_M
#define K_N  GLFW_KEY_N
#define K_O  GLFW_KEY_O
#define K_P  GLFW_KEY_P
#define K_Q  GLFW_KEY_Q
#define K_R  GLFW_KEY_R
#define K_S  GLFW_KEY_S
#define K_T  GLFW_KEY_T
#define K_U  GLFW_KEY_U
#define K_V  GLFW_KEY_V
#define K_W  GLFW_KEY_W
#define K_X  GLFW_KEY_X
#define K_Y  GLFW_KEY_Y
#define K_Z  GLFW_KEY_Z

#define K_ESC    GLFW_KEY_ESCAPE
#define K_SPACE  GLFW_KEY_SPACE
#define K_LSHIFT GLFW_KEY_LEFT_SHIFT

/* input_set_window
 * ----------------
 * Set the window that other input functions use.
 */
void input_set_window(GLFWwindow* window);

/* input_is_pressed
 * ----------------
 * Returns true if key is pressed.
 */
bool input_is_pressed(KeyCode key);

/* input_get_cursor_x
 * ------------------
 * Get the last reported x position of the cursor.
 */
float input_get_cursor_x();

/* input_get_cursor_y
 * ------------------
 * Get the last reported y position of the cursor.
 */
float input_get_cursor_y();

/* input_set_cursor_x
 * ------------------
 * Update the last reported x position of the cursor.
 */
void input_set_cursor_x(float x);

/* input_set_cursor_y
 * ------------------
 * Update the last reported y position of the cursor.
 */
void input_set_cursor_y(float y);
