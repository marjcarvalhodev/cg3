#ifndef SHADER_HPP
#define SHADER_HPP

#include "utils.hpp"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

enum class ShaderType {
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER
};

struct ShaderSources {
  std::string vertex;
  std::string fragment;
};

class MyShader {
private:
  GLuint shaderProgram, vertexShader, fragmentShader, VAO, VBO;
  GLuint compileShader(const char *shaderSource, ShaderType shaderType);
  ShaderSources sources;

  bool bindShaders();
  void validateShader(std::string vertexShaderSource,
                      std::string fragmentShaderSource);

  std::map<std::string, GLint> uniformLocations;

  GLint getUniformLocation(const std::string &name);

public:
  MyShader(ShaderSources sources);
  ~MyShader();

  void use();

  GLuint getProgramID() const;

  void updateGlobalUniforms(const glm::mat4 &viewMat, const glm::mat4 &projMat,
                            const glm::vec3 &cameraPos,
                            const glm::vec3 &lightColorVec);

  void updateShader(const glm::mat4 &modelMat, const glm::mat4 &viewMat,
                    const glm::mat4 &projMat, const glm::vec3 &lightPos,
                    const glm::vec3 &cameraPos, const glm::vec3 &lightColorVec);

  void setTexture(const std::string &name, GLuint textureID, int textureUnit);

  // Set uniform values
  void setUniform(const std::string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                       glm::value_ptr(matrix));
  }

  void setUniform(const std::string &name, const glm::vec3 &vector) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vector));
  }

  void setUniform(const std::string &name, float value) {
    glUniform1f(getUniformLocation(name), value);
  }

  void setUniform(const std::string &name, int value) {
    glUniform1i(getUniformLocation(name), value);
  }
};

#endif