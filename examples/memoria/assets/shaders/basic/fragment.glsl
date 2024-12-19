#version 330 core

in vec2 TexCoords;       // Texture coordinates from vertex shader
out vec4 FragColor;

uniform sampler2D uTexture; // Texture sampler
uniform vec3 materialColor; // Optional base color to mix with texture

void main() {
    vec3 textureColor = texture(uTexture, TexCoords).rgb; // Sample texture
    FragColor = vec4(textureColor * materialColor, 1.0);  // Combine with material color
}