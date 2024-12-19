#version 330 core

in vec3 FragPos;   // World position of the fragment
in vec3 Normal;    // Surface normal at the fragment

out vec4 FragColor;

uniform vec3 viewPos;     // Camera position
uniform vec3 lightPos;    // Light position
uniform vec3 lightColor;  // Light color
uniform samplerCube environmentMap; // Optional environment map for reflections

void main() {
    // Normals
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);

    // 1. Ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. Specular highlights (Phong reflection model)
    float shininess = 128.0;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // 4. Reflections using cubemap
    vec3 envReflect = reflect(-viewDir, norm);
    vec3 reflection = texture(environmentMap, envReflect).rgb;

    // 5. Transparency (blending)
    vec3 glassColor = vec3(0.6, 0.8, 1.0); // Light blue tint for the glass
    float transparency = 0.5; // Controls glass transparency (0 = fully transparent, 1 = opaque)

    vec3 color = ambient + diffuse + specular + 0.5 * reflection; // Add reflections
    color = mix(glassColor, color, 0.5); // Blend base glass color with lighting

    FragColor = vec4(color, transparency); // Final output with transparency
}
