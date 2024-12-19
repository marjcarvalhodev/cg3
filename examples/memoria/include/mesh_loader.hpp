#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "tiny_obj_loader.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

struct MeshData
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    bool isEmpty() const
    {
        return vertices.empty() && normals.empty() && texCoords.empty();
    }
};

class MeshLoader
{
public:
    static MeshData loadModel(const std::string &objPath);
};

#endif
