precision mediump float;

varying vec2 TexCoords;       // Texture coordinates from vertex shader

uniform sampler2D uTexture;   // Texture sampler
uniform vec3 materialColor;   // Optional base color to mix with texture

void main() {
    vec3 textureColor = texture2D(uTexture, TexCoords).rgb; // Sample texture
    gl_FragColor = vec4(textureColor * materialColor, 1.0); // Combine with material color
}
