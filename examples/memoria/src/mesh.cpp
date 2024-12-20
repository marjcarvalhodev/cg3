#include "mesh.hpp"

MyMesh::MyMesh(MeshData meshData)
    : vertexCount(meshData.vertices.size() / 3), indexCount(0) {
  // Create and bind the VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Positions
  if (!meshData.vertices.empty()) {
    GLuint positionVBO;
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(float),
                 meshData.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0); // Index 0 for positions
    glEnableVertexAttribArray(0);
  }

  // Normals
  if (!meshData.normals.empty()) {
    GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, meshData.normals.size() * sizeof(float),
                 meshData.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0); // Index 1 for normals
    glEnableVertexAttribArray(1);
  }

  // Texture Coordinates
  if (!meshData.texCoords.empty()) {
    GLuint texCoordVBO;
    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, meshData.texCoords.size() * sizeof(float),
                 meshData.texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0); // Index 2 for texCoords
    glEnableVertexAttribArray(2);
  }

  if (!meshData.indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 meshData.indices.size() * sizeof(unsigned int),
                 meshData.indices.data(), GL_STATIC_DRAW);

    indexCount = meshData.indices.size();
  }

  // Unbind VAO and VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  computeBounds(meshData.vertices);
}

MyMesh::~MyMesh() { glEnableVertexAttribArray(0); }

void MyMesh::bind() const { glBindVertexArray(VAO); }

void MyMesh::unbind() const { glBindVertexArray(0); }

size_t MyMesh::getVertexCount() const { return vertexCount; }

const glm::mat4 &MyMesh::getModelMatrix() const { return modelMatrix; }

void MyMesh::setModelMatrix(const glm::mat4 &matrix) { modelMatrix = matrix; }

void MyMesh::computeBounds(const std::vector<float> &vertices) {
  glm::vec3 min = glm::vec3(FLT_MAX);
  glm::vec3 max = glm::vec3(-FLT_MIN);

  for (size_t i = 0; i < vertices.size(); i += 3) {
    glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
    min = glm::min(min, vertex);
    max = glm::max(max, vertex);
  }

  minBounds = min;
  maxBounds = max;
}

// eof