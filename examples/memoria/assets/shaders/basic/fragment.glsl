#version 330 core

in vec3 FragPos;         // Fragment position in world space
in vec3 Normal;          // Normal vector in world space
in vec2 TexCoords;       // Texture coordinates from vertex shader

out vec4 FragColor;

uniform vec3 lightPos;   // Position of the light
uniform vec3 viewPos;    // Position of the camera/viewer
uniform vec3 lightColor; // Color of the light
uniform vec3 objectColor; // Base object color
uniform sampler2D uTexture; // Texture sampler
uniform vec3 materialColor; // Base material color
uniform bool hasTexture; // Flag to indicate if texture is used

void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;

    // Handle textured vs. non-textured objects
    vec3 textureColor = hasTexture ? texture(uTexture, TexCoords).rgb : vec3(1.0);
    vec3 finalColor = result * textureColor * materialColor;

    FragColor = vec4(finalColor, 1.0);
}