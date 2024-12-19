#include "shader.hpp"

MyShader::MyShader(ShaderSources sources)
    : sources(sources), shaderProgram(0), vertexShader(0), fragmentShader(0)
{
    // validateShader(sources.vertex, sources.fragment);

    vertexShader = compileShader(sources.vertex.c_str(), ShaderType::Vertex);
    fragmentShader = compileShader(sources.fragment.c_str(), ShaderType::Fragment);

    bindShaders();
}

MyShader::~MyShader()
{
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
    }
}

GLuint MyShader::compileShader(const char *shaderSource, ShaderType shaderType)
{
    GLuint shader = glCreateShader(static_cast<GLenum>(shaderType));
    if (shader == 0)
    {
        throw std::runtime_error("Failed to create shader.");
    }

    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool MyShader::bindShaders()
{
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program linking failed: " << infoLog << std::endl;
        return false; // Indicate failure
    }

    // Delete shaders after linking (no longer needed)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true; // Indicate success
};

void MyShader::use()
{
    glUseProgram(shaderProgram);
}

GLuint MyShader::getProgramID() const
{
    return shaderProgram;
}

void MyShader::updateShader(
    const glm::mat4 modelMat, const glm::mat4 viewMat,
    const glm::mat4 projMat, const glm::vec3 &light, const glm::vec3 &cameraPos)
{
    GLuint modelLoc = glGetUniformLocation(getProgramID(), "uModel");
    GLuint viewLoc = glGetUniformLocation(getProgramID(), "uView");
    GLuint projLoc = glGetUniformLocation(getProgramID(), "uProjection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    GLuint lightLoc = glGetUniformLocation(getProgramID(), "lightPos");
    GLuint viewPos = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint lightColor = glGetUniformLocation(shaderProgram, "lightColor");

    glUniform3fv(lightLoc, 1, glm::value_ptr(light));
    glUniform3fv(viewPos, 1, glm::value_ptr(cameraPos));
    glUniform3fv(lightColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
}

void MyShader::validateShader(std::string vertexShaderSource, std::string fragmentShaderSource)
{
    if (vertexShaderSource.empty())
    {
        throw std::runtime_error("Vertex shader source is empty.");
    }
    if (vertexShaderSource.find("#version") == std::string::npos)
    {
        throw std::runtime_error("Vertex shader source does not contain #version directive.");
    }
    // print("Vertex Shader Source:\n", vertexShaderSource);

    if (fragmentShaderSource.empty())
    {
        throw std::runtime_error("Fragment shader source is empty.");
    }
    if (fragmentShaderSource.find("#version") == std::string::npos)
    {
        throw std::runtime_error("Fragment shader source does not contain #version directive.");
    }
    // print("Fragment Shader Source:\n", fragmentShaderSource);
}

void MyShader::setTexture(const std::string &name, GLuint textureID, int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLuint textureLoc = glGetUniformLocation(getProgramID(), "textureLoc");
    glUniform1i(textureLoc, textureUnit);
}

// eof