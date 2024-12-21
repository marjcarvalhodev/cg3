#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "assets_manager.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <iostream>
#include <memory>
#include <string>

class MyObject {
private:
  std::shared_ptr<MyMesh> mesh;
  std::shared_ptr<MyMesh> meshBox;
  Material material;
  std::shared_ptr<MyShader> shader;
  std::shared_ptr<MyShader> shaderBox;
  GLuint textureId;
  int drawType;
  glm::mat4 modelMatrix;
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;
  std::vector<glm::vec4> boundingBoxCorners;
  std::vector<float> boundingBoxVertices;

  void updateBoundingBox(const glm::vec3 &meshMin, const glm::vec3 &meshMax);

public:
  bool isTransparent;

  MyObject(std::shared_ptr<MyMesh> mesh,
           const Material &material, std::shared_ptr<MyShader> shader,
           std::shared_ptr<MyShader> shaderBox, bool isTransparent = false,
           GLuint textureId = 0, int drawType = GL_TRIANGLES);

  ~MyObject();

  // void render(const glm::mat4 &view, const glm::mat4 &projection,
  //             const glm::vec3 &cameraPos);

  void render(const glm::mat4 &view, const glm::mat4 &projection,
              const glm::vec3 &cameraPos);

  void setModelMatrix(const glm::mat4 &matrix) { modelMatrix = matrix; }

  glm::vec3 getPosition() const { return modelMatrix[3]; }

  void repositionObject(glm::vec3 newPosition);

  void spin(glm::mat4 rotationMatrix) {
    setModelMatrix(rotationMatrix * modelMatrix);
  }

  bool intersectsRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir) const;

  virtual void onClick() { std::cout << "Object clicked!" << std::endl; }
};

#endif // OBJECT_HPP