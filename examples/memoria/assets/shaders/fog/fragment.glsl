#version 330 core

in vec3 FragPos;    // World-space position
in vec3 Normal;     // Normal vector
in vec2 TexCoords;  // Texture coordinates

out vec4 FragColor;

uniform vec3 lightPos;         // Light position
uniform vec3 lightColor;       // Light color
uniform vec3 fogColor;         // Base fog color
uniform float fogStart;        // Fog starts at this distance
uniform float fogEnd;          // Fog ends at this distance
uniform sampler2D uTexture;    // Texture
uniform float time;            // Time for dynamic effect

// Simple hash function for pseudo-randomness
float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

// Pseudo-random noise based on world position
float noise(vec3 position) {
    float n = hash(position.x + position.y * 57.0 + position.z * 113.0);
    return n * 2.0 - 1.0; // Map to [-1, 1]
}

void main() {
    // Lighting calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = 0.1 * lightColor;
    vec3 lighting = ambient + diffuse;

    vec3 textureColor = texture(uTexture, TexCoords).rgb;
    vec3 finalColor = lighting * textureColor;

    // Fog calculations
    float distance = length(FragPos);
    float fogFactor = clamp((distance - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

    // Add dynamic noise to fog color
    float noiseValue = noise(FragPos + vec3(time * 0.1));
    vec3 dynamicFogColor = fogColor + noiseValue * 0.1; // Slight variation in fog color

    // Blend final color with fog
    vec3 colorWithFog = mix(finalColor, dynamicFogColor, fogFactor);

    FragColor = vec4(colorWithFog, 1.0);
}
