/* input.h
 *
 * Retrieve input state. A wrapper around glfw.
 *
 */

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef int KeyCode;

#define K_ESC    GLFW_KEY_ESCAPE
#define K_SPACE  GLFW_KEY_SPACE
#define K_LSHIFT GLFW_KEY_LEFT_SHIFT
#define K_W      GLFW_KEY_W
#define K_A      GLFW_KEY_A
#define K_S      GLFW_KEY_S
#define K_D      GLFW_KEY_D

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
