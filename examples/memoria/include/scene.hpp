#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "factories.hpp"
#include "grid.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "window.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class MyScene {
private:
  std::unordered_map<std::string, std::shared_ptr<MyShader>> scene_shaders;
  std::unordered_map<std::string, std::shared_ptr<MyObject>> scene_objects;
  std::unordered_map<std::string, std::shared_ptr<MyMesh>> scene_meshes;
  MyCamera camera;
  GameGrid grid;

  GLuint gridVAO = 0, gridVBO = 0;
  int gridVertexCount = 0;

  // Initializer Functions
  void initializeShaders(AssetsManager &assetsManager);
  void initializeModels(AssetsManager &assetsManager);
  void initializeObjects(AssetsManager &assetsManager);

public:
  MyScene(const MyCamera &camera, const GameGrid &grid);
  ~MyScene();

  static MyScene setupScene(MyCamera &camera, AssetsManager &assetsManager);

  void addSceneObjects(const std::string &key,
                       const std::shared_ptr<MyObject> &object) {
    scene_objects[key] = object;
  }

  std::unordered_map<std::string, std::shared_ptr<MyObject>>
  getAllSceneObjects() {
    return scene_objects;
  }

  std::shared_ptr<MyObject> getSceneObject(const std::string &key) const {
    auto it = scene_objects.find(key);
    if (it != scene_objects.end())
      return it->second;
    throw std::runtime_error("Scene object not found: " + key);
  }

  void addSceneShaders(const std::string &key,
                       const std::shared_ptr<MyShader> &shader) {
    scene_shaders[key] = shader;
  }

  std::unordered_map<std::string, std::shared_ptr<MyShader>>
  getAllSceneShaders() {
    return scene_shaders;
  }

  std::shared_ptr<MyShader> getSceneShader(const std::string &key) const {
    auto it = scene_shaders.find(key);
    if (it != scene_shaders.end())
      return it->second;
    throw std::runtime_error("Scene object not found: " + key);
  }

  void addSceneMeshes(const std::string &key,
                       const std::shared_ptr<MyMesh> &mesh) {
    scene_meshes[key] = mesh;
  }

  std::shared_ptr<MyMesh> getSceneMesh(const std::string &key) const {
    auto it = scene_meshes.find(key);
    if (it != scene_meshes.end())
      return it->second;
    throw std::runtime_error("Scene mesh not found: " + key);
  }

  void animateObject(std::shared_ptr<MyObject> object, float deltaTime);

  void renderScene(MyWindow &window) const;

  void handleMouseClick(int mouseX, int mouseY, const MyCamera &camera,
                        int windowWidth, int windowHeight);

  void moveCamera(MyCamera &camera, const glm::vec3 &direction) {
    camera.translate(direction);
  }

  void handleSpacePress() {
    // std::cout << "Space bar pressed:" << std::endl;
    // Add game logic here
  }

  void handleEnterPress() {
    // std::cout << "Enter key pressed:" << std::endl;
    // Add game logic here
  }
};

#endif // SCENE_HPP