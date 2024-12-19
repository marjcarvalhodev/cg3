#version 330 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

out vec3 FragPos;   // Fragment position in world space
out vec3 Normal;    // Normal vector in world space

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    // Transform vertex position to world space
    FragPos = vec3(uModel * vec4(aPos, 1.0));

    // Transform normal to world space
    Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal);

    // Transform vertex to clip space
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}