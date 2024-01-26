#include <glad/glad.h>

typedef struct VAO {
    unsigned int id;
    unsigned int ebo;
} VAO;

/* vao_init
 * --------
 * Generate buffers. and save their IDs in vao.
 */
void vao_init(VAO* vao);

/* vao_bind
 * --------
 * Bind the vao referenced by vaoID.
 */
void vao_bind(VAO* vao);

/* vao_unbind
 * ----------
 * Unbind the currently bound vao and vbo.
 */
void vao_unbind();

/* vao_ebo_data
 * ------------
 * Supply the vao's ebo with an index array.
 */
void vao_ebo_data(
        VAO* vao,
        GLsizeiptr size,
        const GLvoid* data,
        GLenum usage);

/* vao_vertex_attrib
 * -----------------
 * Specify a vertex attribute for this vao.
 */
void vao_vertex_attrib(
        VAO* vao,
        GLuint index,
        GLint  size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid* ptr);


