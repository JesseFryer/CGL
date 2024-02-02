#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec3 aNormal;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPos;
uniform vec3 camPos;

out vec3 objColour;
out float alpha;
out float diffuseStr;
out float specularStr;

void main() {
    objColour = vec3(aColour.x, aColour.y, aColour.z);
    alpha = aColour.w;

    vec3 diffuseDir = normalize(lightPos - aPosition);
    diffuseStr = max(dot(diffuseDir, aNormal), 0.0);

    vec3 viewDir = normalize(camPos - aPosition);
    vec3 reflectDir = reflect(-diffuseDir, aNormal);
    specularStr = pow(max(dot(viewDir, reflectDir), 0.0), 64);

    gl_Position = proj * view * vec4(aPosition, 1.0);
}
