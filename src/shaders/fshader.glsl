#version 330 core

in vec3 fPos;
in vec3 fLightPos;
in vec3 fCamPos;
in vec3 fNormal;
in vec3 fObjColour;
in float fObjAlpha;

out vec4 fragColour;

void main() {
    vec3 lightColour = vec3(0.8, 1.0, 1.0);

    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColour;

    vec3 diffuseDir = normalize(fLightPos - fPos);
    float diffuseStr = max(dot(diffuseDir, fNormal), 0.0);
    vec3 diffuse = diffuseStr * lightColour;

    float specScalar = 0.5;
    vec3 viewDir = normalize(fCamPos - fPos);
    vec3 reflectDir = reflect(-diffuseDir, fNormal);
    float specularStr = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specScalar * specularStr * lightColour;

    vec3 result = (ambient + diffuse + specular) * fObjColour;
    fragColour = vec4(result, fObjAlpha);
}
