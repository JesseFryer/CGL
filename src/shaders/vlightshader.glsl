
#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColour;
layout (location = 2) in vec3 vNormal;

uniform mat4 view;
uniform mat4 proj;

out vec4 fObjColour;

void main() {
    fObjColour = vColour;
    gl_Position = proj * view * vec4(vPos, 1.0);
}
