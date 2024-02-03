#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColour;
layout (location = 2) in vec3 vNormal;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 vLightPos;
uniform vec3 vCamPos;

out vec3 fPos;
out vec3 fLightPos;
out vec3 fCamPos;
out vec3 fNormal;
out vec3 fObjColour;
out float fObjAlpha;

void main() {
    fPos = vPos;
    fLightPos = vLightPos;
    fCamPos = vCamPos;
    fNormal = vNormal;
    fObjColour = vec3(vColour.x, vColour.y, vColour.z);
    fObjAlpha = vColour.w;

    gl_Position = proj * view * vec4(vPos, 1.0);
}
