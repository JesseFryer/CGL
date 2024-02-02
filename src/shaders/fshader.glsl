#version 330 core

in vec3 objColour;
in float alpha;
in float diffuseStr;
in float specularStr;

out vec4 fragColour;

void main() {
    vec3 lightColour = vec3(1.0, 1.0, 1.0);

    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColour;

    vec3 diffuse = diffuseStr * lightColour;

    float specScalar = 0.5;
    vec3 specular = specScalar * specularStr * lightColour;

    fragColour = vec4(
            (diffuse + ambient + specular) * objColour,
            alpha);
}
