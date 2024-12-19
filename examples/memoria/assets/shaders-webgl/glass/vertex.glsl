precision mediump float;

attribute vec3 aPos;      // Vertex position
attribute vec3 aNormal;   // Vertex normal

varying vec3 FragPos;     // Pass to fragment shader
varying vec3 Normal;      // Pass to fragment shader

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0)); // Transform position
    Normal = mat3(transpose(inverse(uModel))) * aNormal; // Transform normal
    gl_Position = uProjection * uView * vec4(FragPos, 1.0); // Final position
}
