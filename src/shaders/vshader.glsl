#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec3 aNormal;

uniform mat4 view;
uniform mat4 proj;

out vec4 colour;

void main() {
    colour = vec4(aPosition, 1.0);
    gl_Position = proj * view * vec4(aPosition, 1.0);
}
