#version 330 core

layout(location = 0) in vec3 aPos;     // Vertex position
layout(location = 1) in vec3 aNormal;     // Vertex position
layout(location = 2) in vec2 aTexCoord; // Texture coordinates

out vec3 FragPos;   // World space position of the fragment
out vec3 Normal;    // Normal at the fragment
out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uvScale;

void main() {
    TexCoords = aTexCoord * 1.0; // Pass the texture coordinates
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal; // Transform normals
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}