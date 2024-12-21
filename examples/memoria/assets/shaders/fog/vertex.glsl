#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Normal vector
layout(location = 2) in vec2 aTexCoord; // Texture coordinates

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0));  // Transform vertex to world space
    Normal = mat3(transpose(inverse(uModel))) * aNormal; // Correct normal transformation
    TexCoords = aTexCoord;  // Pass texture coordinates
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
