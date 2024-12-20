#ifndef MESH_HPP
#define MESH_HPP

#include "mesh_loader.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

struct Material {
  glm::vec3 color;
  float shininess; // For shiny/rough differentiation
};

class MyMesh {
private:
  GLuint VAO, VBO, EBO;
  size_t vertexCount, indexCount;
  glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix by default
  glm::vec3 minBounds;
  glm::vec3 maxBounds;

  void computeBounds(const std::vector<float> &vertices);

public:
  MyMesh(MeshData meshData);
  ~MyMesh();

  void bind() const;
  void unbind() const;
  size_t getVertexCount() const;
  size_t getIndexCount() const { return indexCount; }

  const glm::mat4 &getModelMatrix() const;
  void setModelMatrix(const glm::mat4 &matrix);

  glm::vec3 getMinBounds() const { return minBounds; }
  glm::vec3 getMaxBounds() const { return maxBounds; }
};

#endif