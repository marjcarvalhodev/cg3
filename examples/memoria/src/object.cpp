#include "object.hpp"

MyObject::MyObject(std::shared_ptr<MyMesh> mesh, const Material &material,
                   std::shared_ptr<MyShader> shader,
                   std::shared_ptr<MyShader> shaderBox, bool isTransparent,
                   GLuint textureId, int drawType)
    : mesh(mesh), material(material), shader(shader), shaderBox(shaderBox),
      textureId(textureId), drawType(drawType), modelMatrix(glm::mat4(1.0f)),
      isTransparent(isTransparent) {

  updateBoundingBox(mesh->getMinBounds(), mesh->getMaxBounds());

  MeshData meshData = createBoundingBoxMeshData(boundingBoxCorners.data());
  meshBox = std::make_shared<MyMesh>(meshData);
}

MyObject::~MyObject() {}

void MyObject::render(const glm::mat4 &view, const glm::mat4 &projection,
                      const glm::vec3 &cameraPos) {

  glm::vec3 lightPos = glm::vec3(5.0f, 10.0f, 5.0f); // A light above the scene
  glm::vec3 lightColor = glm::vec3(1.0f);            // White light

  shader->use();
  shader->updateShader(modelMatrix, view, projection, lightPos, cameraPos,
                       lightColor);

  // Bind texture if available
  if (textureId) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader->setUniform("uTexture", 0);
    shader->setUniform("hasTexture", true);
  } else {
    shader->setUniform("hasTexture", false);
  }

  // Set material properties
  shader->setUniform("materialColor", material.color);
  shader->setUniform("materialShininess", material.shininess);

  shader->setUniform("time", SDL_GetTicks() / 1000.0f); // Time in seconds
  shader->setUniform("fogColor", glm::vec3(0.5f, 0.5f, 0.5f));
  shader->setUniform("fogStart", 5.0f);
  shader->setUniform("fogEnd", 20.0f);

  // Bind and draw the mesh
  mesh->bind();
  glDrawElements(drawType, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
  mesh->unbind();

  // Render the bounding box
  if (meshBox && shaderBox) {
    shaderBox->use(); // Activate the shader for the bounding box
    shaderBox->updateShader(modelMatrix, view, projection, lightPos, cameraPos,
                            glm::vec3(1.0f, 0.0f, 0.0f));

    meshBox->bind();
    glDrawElements(GL_LINES, meshBox->getIndexCount(), GL_UNSIGNED_INT, 0);
    meshBox->unbind();
  }
}

void

void MyObject::repositionObject(glm::vec3 newPosition) {
  setModelMatrix(glm::translate(glm::mat4(1.0f), newPosition));
}

bool MyObject::intersectsRay(const glm::vec3 &rayOrigin,
                             const glm::vec3 &rayDir) const {

  float tmin = (boundingBoxMin.x - rayOrigin.x) / rayDir.x;
  float tmax = (boundingBoxMax.x - rayOrigin.x) / rayDir.x;
  if (tmin > tmax)
    std::swap(tmin, tmax);

  float tymin = (boundingBoxMin.y - rayOrigin.y) / rayDir.y;
  float tymax = (boundingBoxMax.y - rayOrigin.y) / rayDir.y;
  if (tymin > tymax)
    std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  float tzmin = (boundingBoxMin.z - rayOrigin.z) / rayDir.z;
  float tzmax = (boundingBoxMax.z - rayOrigin.z) / rayDir.z;
  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);

  return (tmin <= tzmax) && (tzmin <= tmax);
}

void MyObject::updateBoundingBox(const glm::vec3 &meshMin,
                                 const glm::vec3 &meshMax) {
  // Transform the bounding box by the model matrix
  boundingBoxCorners = {
      glm::vec4(meshMin.x, meshMin.y, meshMin.z, 1.0f),
      glm::vec4(meshMin.x, meshMin.y, meshMax.z, 1.0f),
      glm::vec4(meshMin.x, meshMax.y, meshMin.z, 1.0f),
      glm::vec4(meshMin.x, meshMax.y, meshMax.z, 1.0f),
      glm::vec4(meshMax.x, meshMin.y, meshMin.z, 1.0f),
      glm::vec4(meshMax.x, meshMin.y, meshMax.z, 1.0f),
      glm::vec4(meshMax.x, meshMax.y, meshMin.z, 1.0f),
      glm::vec4(meshMax.x, meshMax.y, meshMax.z, 1.0f),
  };

  glm::vec3 worldMin = glm::vec3(FLT_MAX);
  glm::vec3 worldMax = glm::vec3(-FLT_MAX);

  for (const auto &corner : boundingBoxCorners) {
    glm::vec3 transformedCorner = glm::vec3(modelMatrix * corner);
    worldMin = glm::min(worldMin, transformedCorner);
    worldMax = glm::max(worldMax, transformedCorner);
  }

  boundingBoxMin = worldMin;
  boundingBoxMax = worldMax;
}

// eof