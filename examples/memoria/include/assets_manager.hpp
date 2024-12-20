#ifndef ASSETS_MANAGER_HPP
#define ASSETS_MANAGER_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "texture_loader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
// #include <format>

class AssetsManager {
private:
  std::string basePath;

  std::unordered_map<std::string, std::shared_ptr<MyMesh>> models;
  std::unordered_map<std::string, std::shared_ptr<MyShader>> shaders;
  std::unordered_map<std::string, Material> materials;
  std::unordered_map<std::string, GLuint> textures;

public:
  explicit AssetsManager();

  ~AssetsManager();

  std::string readFile(const std::string &fileName) const;

  void loadModel(const std::string &key, const std::string &modelName);

  void loadShader(const std::string &key, const std::string &shaderName);

  void addMaterial(const std::string &key, const Material &material) {
    materials[key] = material;
  }

  std::shared_ptr<MyMesh> getModel(const std::string &key) {
    return models.at(key);
  }

  std::shared_ptr<MyShader> getShader(const std::string &key) {
    return shaders.at(key);
  }

  Material getMaterial(const std::string &key) { return materials.at(key); }

  void preloadAllTextures();

  GLuint getTexture(const std::string &textureName) {
    return textures.at(textureName);
  }
};

#endif