/* input.h
 *
 * Retrieve input state. A wrapper around glfw.
 *
 */

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef int KeyCode;

#define K_SPACE 32

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
