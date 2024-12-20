#version 330 core

in vec3 FragPos;    // World-space position
in vec3 Normal;     // Normal vector
in vec2 TexCoords;  // Texture coordinates

out vec4 FragColor;

uniform vec3 viewPos;         // Camera position
uniform vec3 lightPos;        // Light position
uniform vec3 lightColor;      // Light color
uniform vec3 fogColor;        // Fog color
uniform float fogStart;       // Fog starts at this distance
uniform float fogEnd;         // Fog ends at this distance
uniform sampler2D uTexture;   // Texture

void main() {
    // Lighting calculations (same as before)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = diffuse;  // Simplified lighting for now

    // Fog calculations
    float distance = length(viewPos - FragPos);
    float fogFactor = clamp((distance - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    vec3 color = mix(result, fogColor, fogFactor);

    FragColor = vec4(color, 1.0);
}
