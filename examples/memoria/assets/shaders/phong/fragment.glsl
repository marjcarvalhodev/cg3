#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

#define MAX_LIGHTS 4

uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform vec3 materialColor;
uniform float materialShininess;
uniform float ambientStrength;
uniform float specularStrength;

uniform sampler2D uTexture;
uniform bool hasTexture;

vec3 CalcAmbient(vec3 lightColor, float ambientStrength) {
    return ambientStrength * lightColor;
}

vec3 CalcDiffuse(vec3 norm, vec3 lightDir, vec3 lightColor) {
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

vec3 CalcSpecular(vec3 norm, vec3 lightDir, vec3 viewDir, vec3 lightColor, float specularStrength, float shininess) {
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularStrength * spec * lightColor;
}

vec3 GetBaseColor(bool hasTexture, sampler2D textureSampler, vec2 texCoords, vec3 materialColor) {
    if (hasTexture) {
        return texture(textureSampler, texCoords).rgb * materialColor;
    } else {
        return materialColor;
    }
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 baseColor = GetBaseColor(hasTexture, uTexture, TexCoords, materialColor);

    vec3 result = vec3(0.0);

    // for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lightPos[0] - FragPos);

        vec3 lightColorI = lightColor[0];

        vec3 ambient = CalcAmbient(lightColorI, ambientStrength);
        vec3 diffuse = CalcDiffuse(norm, lightDir, lightColorI);
        vec3 specular = CalcSpecular(norm, lightDir, viewDir, lightColorI, specularStrength, materialShininess);

        result += (ambient + diffuse + specular);
    // }

    result *= baseColor;
    FragColor = vec4(result, 1.0);
}
