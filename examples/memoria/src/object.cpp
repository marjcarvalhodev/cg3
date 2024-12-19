#include "object.hpp"

MyObject::MyObject(std::shared_ptr<MyMesh> mesh, const Material &material,
                   std::shared_ptr<MyShader> shader, bool isTransparent,
                   GLuint textureId)
    : mesh(mesh), material(material), shader(shader),
      modelMatrix(glm::mat4(1.0f)), isTransparent(isTransparent),
      textureId(textureId) {}

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

  // Bind and draw the mesh
  mesh->bind();
  glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
  mesh->unbind();
}

void MyObject::repositionObject(glm::vec3 newPosition) {
  setModelMatrix(glm::translate(glm::mat4(1.0f), newPosition));
}

// eof