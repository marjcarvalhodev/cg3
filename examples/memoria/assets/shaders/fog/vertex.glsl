#version 330 core

layout(location = 0) in vec3 aPos;     // Vertex position
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    TexCoords = aTexCoord * 10.0;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}