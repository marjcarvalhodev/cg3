#ifndef GRID_HPP
#define GRID_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class GameGrid {
private:
  int gridSize;
  std::vector<std::vector<int>> grid;
  float cellSize;

public:
  GameGrid(int gridSize, float cellSize)
      : gridSize(gridSize), grid(gridSize, std::vector<int>(gridSize, 0)),
        cellSize(cellSize) {}

  int getGridSize() const { return gridSize; }
  int getCellSize() const { return cellSize; }

  int getCell(int x, int z) const {
    if (x < 0 || x >= gridSize || z < 0 || z >= gridSize) {
      std::cerr << "Grid cell out of bounds!" << std::endl;
      return -1;
    }
    return grid[z][x];
  }

  void setCell(int x, int z, int value) {
    if (x < 0 || x >= gridSize || z < 0 || z >= gridSize) {
      std::cerr << "Grid cell out of bounds!" << std::endl;
      return;
    }
    grid[z][x] = value;
  }

  void printGrid() const {
    for (const auto &row : grid) {
      for (const auto &cell : row) {
        std::cout << cell << " ";
      }
      std::cout << std::endl;
    }
  }

  std::pair<int, int> worldToGridCoordinates(const glm::vec3 &worldPosition,
                                             float cellSize) const {
    int gridX = static_cast<int>(
        (worldPosition.x + (gridSize * cellSize) / 2.0f) / cellSize);
    int gridZ = static_cast<int>(
        (worldPosition.z + (gridSize * cellSize) / 2.0f) / cellSize);

    return {gridX, gridZ};
  }

  glm::vec3 gridToWorldCoordinates(int gridX, int gridZ, float cellSize) const {
    float worldX =
        gridX * cellSize - (gridSize * cellSize) / 2.0f + cellSize / 2.0f;
    float worldZ =
        gridZ * cellSize - (gridSize * cellSize) / 2.0f + cellSize / 2.0f;

    return {worldX, 0.0f, worldZ}; // Assuming Y is 0 for the floor
  }

  bool intersectsRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir,
                     glm::vec2 &hitCell, float cellSize) const {
    // Assume the floor lies on the XZ plane (y = 0)
    if (std::abs(rayDir.y) < 1e-6) {
      // The ray is parallel to the floor
      return false;
    }

    // Calculate the intersection point with the floor (y = 0)
    float t = -rayOrigin.y / rayDir.y;
    if (t < 0) {
      // The ray is pointing away from the floor
      return false;
    }

    glm::vec3 hitPoint = rayOrigin + t * rayDir;

    // Map the hit point to grid coordinates
    int gridX = static_cast<int>((hitPoint.x + (gridSize * cellSize) / 2.0f) /
                                 cellSize);
    int gridZ = static_cast<int>((hitPoint.z + (gridSize * cellSize) / 2.0f) /
                                 cellSize);

    // Check if the hit cell is within bounds
    if (gridX < 0 || gridX >= gridSize || gridZ < 0 || gridZ >= gridSize) {
      return false;
    }

    hitCell = glm::vec2(gridX, gridZ);
    return true;
  }
};

#endif
