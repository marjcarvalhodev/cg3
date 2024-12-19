precision mediump float;

attribute vec3 aPos;         // Vertex position
attribute vec3 aNormal;      // Vertex normal
attribute vec2 aTexCoord;    // Texture coordinates

varying vec2 TexCoords;      // Pass to fragment shader

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uvScale;

void main() {
    TexCoords = aTexCoord * uvScale; // Scale the texture coordinates
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
