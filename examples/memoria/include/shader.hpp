#ifndef SHADER_HPP
#define SHADER_HPP

#include "utils.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

enum class ShaderType
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

struct ShaderSources
{
    std::string vertex;
    std::string fragment;
};

class MyShader
{
private:
    GLuint shaderProgram, vertexShader, fragmentShader, VAO, VBO;
    GLuint compileShader(const char *shaderSource, ShaderType shaderType);
    ShaderSources sources;

    bool bindShaders();
    void validateShader(std::string vertexShaderSource, std::string fragmentShaderSource);

public:
    MyShader(ShaderSources sources);
    ~MyShader();

    void use();

    GLuint getProgramID() const;
    
    void updateShader(
        const glm::mat4 modelMat, const glm::mat4 viewMat,
        const glm::mat4 projMat, const glm::vec3 &light, const glm::vec3 &cameraPos);

    void setTexture(const std::string &name, GLuint textureID, int textureUnit);
};

#endif