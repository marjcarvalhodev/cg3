#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

#define MAX_LIGHTS 4

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform int numLights;

uniform vec3 viewPos;
uniform vec3 materialColor;
uniform float materialShininess;
uniform float ambientStrength;
uniform float specularStrength;

uniform sampler2D uTexture;
uniform bool hasTexture;
uniform samplerCube environmentMap; // Optional environment map for reflections

vec3 CalcAmbient(vec3 lightColor, float ambientStrength) {
    return ambientStrength * lightColor;
}

vec3 CalcDiffuse(vec3 norm, vec3 lightDir, vec3 lightColor) {
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

vec3 CalcSpecular(vec3 norm, vec3 lightDir, vec3 viewDir, vec3 lightColor, float specularStrength, float shininess) {
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularStrength * spec * lightColor;
}

vec3 GetBaseColor(bool hasTexture, sampler2D textureSampler, vec2 texCoords, vec3 materialColor) {
    if (hasTexture) {
        return texture(textureSampler, texCoords).rgb * materialColor;
    } else {
        return materialColor;
    }
}

void main() {
    // Normals
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 ambient = CalcAmbient(lightColor, ambientStrength);
    vec3 diffuse = CalcDiffuse(norm, lightDir, lightColor);
    vec3 specular = CalcSpecular(norm, lightDir, viewDir, lightColor, specularStrength, materialShininess);

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
