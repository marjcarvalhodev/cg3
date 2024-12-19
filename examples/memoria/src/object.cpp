#include "object.hpp"

MyObject::MyObject(
    std::shared_ptr<MyMesh> mesh, const Material &material,
    std::shared_ptr<MyShader> shader, bool isTransparent, std::string textureName)
    : mesh(mesh), material(material), shader(shader),
      modelMatrix(glm::mat4(1.0f)), isTransparent(isTransparent), textureName(textureName)
{
    if (!textureName.empty())
    {
        setTextureID(textureName);
    }
}

MyObject::~MyObject()
{
}

void MyObject::render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPos)
{
    shader->use();
    shader->updateShader(modelMatrix, view, projection, glm::vec3(10.0), cameraPos);

    // Bind texture if available
    if (textureID)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shader->getProgramID(), "uTexture"), 0);
    }

    GLuint colorLoc = glGetUniformLocation(shader->getProgramID(), "materialColor");
    glUniform3fv(colorLoc, 1, glm::value_ptr(material.color));

    GLuint shininessLoc = glGetUniformLocation(shader->getProgramID(), "materialShininess");
    glUniform1f(shininessLoc, material.shininess);

    mesh->bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
    mesh->unbind();
}

void MyObject::repositionObject(glm::vec3 newPosition)
{
    setModelMatrix(glm::translate(glm::mat4(1.0f), newPosition));
}

void MyObject::setTextureID(std::string &textureName)
{

    textureID = TextureLoader::loadTextureFromFile(textureName);
}

// eof