#ifndef UTILS_HPP
#define UTILS_HPP

#include "grid.hpp"
#include "mesh_loader.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

inline void printErr(const std::exception &e) {
  std::cerr << "Error: " << e.what() << std::endl;
}

// Variadic template function for printing multiple arguments
template <typename T, typename... Args>
inline void print(const T &first, const Args &...rest) {
  std::cout << first;
  ((std::cout << " " << rest), ...); // Fold expression to print all arguments
  std::cout << std::endl;
}

inline glm::vec3 interpolate(const glm::vec3 &start, const glm::vec3 &end,
                             float alpha) {
  alpha = glm::clamp(alpha, 0.0f, 1.0f); // Ensure alpha is clamped to [0, 1]
  return start + alpha * (end - start);
}

inline bool rayIntersectsAABB(const glm::vec3 &rayOrigin,
                              const glm::vec3 &rayDir, const glm::vec3 &min,
                              const glm::vec3 &max) {
  float tmin = (min.x - rayOrigin.x) / rayDir.x;
  float tmax = (max.x - rayOrigin.x) / rayDir.x;
  if (tmin > tmax)
    std::swap(tmin, tmax);

  float tymin = (min.y - rayOrigin.y) / rayDir.y;
  float tymax = (max.y - rayOrigin.y) / rayDir.y;
  if (tymin > tymax)
    std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  float tzmin = (min.z - rayOrigin.z) / rayDir.z;
  float tzmax = (max.z - rayOrigin.z) / rayDir.z;
  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);

  return (tmin <= tzmax) && (tzmin <= tmax);
}

inline glm::vec3 calculateRayDirection(int mouseX, int mouseY, int windowWidth,
                                       int windowHeight,
                                       const glm::mat4 &viewMatrix,
                                       const glm::mat4 &projectionMatrix) {
  // Convert screen coordinates to normalized device coordinates (NDC)
  float ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
  float ndcY = 1.0f - (2.0f * mouseY) / windowHeight; // Invert Y for NDC
  glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);

  // Convert to camera/view space
  glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
  eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

  // Convert to world space
  glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * eyeCoords);
  rayWorld = glm::normalize(rayWorld);

  return rayWorld;
}

inline MeshData createMeshDataFromVertices(const std::vector<float> &vertices) {
  MeshData meshData;

  for (size_t i = 0; i < vertices.size(); i += 8) {
    // Extract vertex positions
    meshData.vertices.push_back(vertices[i]);
    meshData.vertices.push_back(vertices[i + 1]);
    meshData.vertices.push_back(vertices[i + 2]);

    // Extract normals
    meshData.normals.push_back(vertices[i + 3]);
    meshData.normals.push_back(vertices[i + 4]);
    meshData.normals.push_back(vertices[i + 5]);

    // Extract texture coordinates
    meshData.texCoords.push_back(vertices[i + 6]);
    meshData.texCoords.push_back(vertices[i + 7]);
  }

  return meshData;
}

inline MeshData createBoundingBoxMeshData(const glm::vec4 corners[8]) {
  MeshData meshData;

  // Add vertices
  for (const auto &corner : corners) {
    meshData.vertices.push_back(corner.x);
    meshData.vertices.push_back(corner.y);
    meshData.vertices.push_back(corner.z);

    // Add dummy normals and tex coords (not used for lines)
    meshData.normals.push_back(0.0f);
    meshData.normals.push_back(0.0f);
    meshData.normals.push_back(0.0f);

    meshData.texCoords.push_back(0.0f);
    meshData.texCoords.push_back(0.0f);
  }

  // Add indices for lines
  meshData.indices = {
      0, 1, 1, 5, 5, 4, 4, 0, // Bottom face
      2, 3, 3, 7, 7, 6, 6, 2, // Top face
      0, 2, 1, 3, 5, 7, 4, 6  // Vertical edges
  };

  return meshData;
}

inline std::vector<float> initGridlines(GameGrid &grid) {
  std::vector<float> gridVertices;
  float cellSize = grid.getCellSize();
  int gridSize = grid.getGridSize();

  for (int i = -gridSize / 2; i <= gridSize / 2; ++i) {
    // Horizontal lines
    gridVertices.push_back(-cellSize * gridSize / 2);
    gridVertices.push_back(0.01f);
    gridVertices.push_back(i * cellSize);

    gridVertices.push_back(cellSize * gridSize / 2);
    gridVertices.push_back(0.01f);
    gridVertices.push_back(i * cellSize);

    // Vertical lines
    gridVertices.push_back(i * cellSize);
    gridVertices.push_back(0.01f);
    gridVertices.push_back(-cellSize * gridSize / 2);

    gridVertices.push_back(i * cellSize);
    gridVertices.push_back(0.01f);
    gridVertices.push_back(cellSize * gridSize / 2);
  }

  return gridVertices;
}

#endif