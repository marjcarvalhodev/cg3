#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    vec3 textureColor = texture(uTexture, TexCoords).rgb;
    FragColor = vec4(textureColor, 1.0);
}