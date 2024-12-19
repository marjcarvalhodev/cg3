#define TINYOBJLOADER_IMPLEMENTATION
#include "mesh_loader.hpp"

MeshData MeshLoader::loadModel(const std::string &objPath)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ""; // Path to look for .mtl files
    reader_config.triangulate = true;   // Automatically triangulate faces

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(objPath, reader_config))
    {
        if (!reader.Warning().empty())
        {
            std::cerr << "Warning: " << reader.Warning() << std::endl;
        }
        if (!reader.Error().empty())
        {
            std::cerr << "Error: " << reader.Error() << std::endl;
            return {}; // Return an empty MeshData
        }
        std::cerr << "Failed to load .obj file." << std::endl;
        return {};
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();

    MeshData meshData;

    // Extract vertex data
    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            // Vertex positions
            meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]); // x
            meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]); // y
            meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]); // z

            // Normals
            if (!attrib.normals.empty() && index.normal_index >= 0)
            {
                meshData.normals.push_back(attrib.normals[3 * index.normal_index + 0]); // nx
                meshData.normals.push_back(attrib.normals[3 * index.normal_index + 1]); // ny
                meshData.normals.push_back(attrib.normals[3 * index.normal_index + 2]); // nz
            }

            // Texture coordinates
            if (!attrib.texcoords.empty() && index.texcoord_index >= 0)
            {
                meshData.texCoords.push_back(attrib.texcoords[2 * index.texcoord_index + 0]); // u
                meshData.texCoords.push_back(attrib.texcoords[2 * index.texcoord_index + 1]); // v
            }
        }
    }

    return meshData;
}
