#ifndef OBJECT_H
#define OBJECT_H

#include "vao.h"
#include "clm.h"

typedef struct {
    clmVec3 position;
    VAO* vao;
    unsigned int vbo;
} Object;

/* obj_set_vbo
 * -----------
 * Set the vertices object will use to render.
 */
void obj_set_vbo(Object* obj, unsigned int vbo);

/* obj_set_vao
 * -----------
 * Set the vao object will use to render.
 */
void obj_set_vao(Object* obj, VAO* vao);

/* obj_set_pos
 * -----------
 * Set the position of the object in world space.
 */
void obj_set_pos(Object* obj, clmVec3 position);

/* obj_render
 * ----------
 * Render the object.
 */
void obj_render(Object* obj, unsigned int shader);

#endif
