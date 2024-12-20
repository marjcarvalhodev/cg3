precision mediump float;

varying vec3 FragPos;   // World position of the fragment
varying vec3 Normal;    // Surface normal at the fragment

uniform vec3 viewPos;     // Camera position
uniform vec3 lightPos;    // Light position
uniform vec3 lightColor;  // Light color
uniform samplerCube environmentMap; // Cubemap for reflections

void main() {
    // Normals
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);

    // 1. Ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. Specular highlights (Phong reflection model)
    float shininess = 128.0;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // 4. Reflections using cubemap
    vec3 envReflect = reflect(-
