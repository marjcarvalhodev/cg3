#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <vector>

std::vector<float> floorVertices = {
    // Positions          // Normals          // Texture Coords
    -5.0f, 0.0f, -5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     5.0f, 0.0f, -5.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     5.0f, 0.0f,  5.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -5.0f, 0.0f,  5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f
};

std::vector<unsigned int> floorIndices = {
    0, 1, 2,
    0, 2, 3
};

#endif