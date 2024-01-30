#include "object.h"

void obj_set_vbo(Object* obj, unsigned int vbo) {
    obj->vbo = vbo;
}

void obj_set_vao(Object* obj, VAO* vao) {
    obj->vao = vao;
}

void obj_set_pos(Object* obj, clmVec3 position) {
    obj->position[0] = position[0];
    obj->position[1] = position[1];
    obj->position[2] = position[2];
}

void obj_render(Object* obj, unsigned int shader) {
    // Bind.
    vao_bind(obj->vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);

    clmMat4 model;
    clm_mat4_identity(model);
    clm_mat4_translate(model, obj->position);
    unsigned int modelLoc = glGetUniformLocation(
            shader, "model");
    glUniformMatrix4fv(modelLoc,
            1, GL_FALSE, model);

    glDrawElements(GL_TRIANGLES, 
            obj->vao->indicesCount, 
            GL_UNSIGNED_INT, 
            0);
    
    // Unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vao_unbind(obj->vao);
}
