#ifndef MESH_HPP
#define MESH_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "mesh_loader.hpp"

struct Material {
    glm::vec3 color;
    float shininess; // For shiny/rough differentiation
};

class MyMesh
{
private:
    GLuint VAO, VBO;
    size_t vertexCount;
    glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix by default

public:
    MyMesh(MeshData meshData);
    ~MyMesh();

    void bind() const;
    void unbind() const;
    size_t getVertexCount() const;
    const glm::mat4 &getModelMatrix() const;
    void setModelMatrix(const glm::mat4 &matrix);
};

#endif