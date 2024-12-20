#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include "camera.hpp"
#include "window.hpp"
#include "object.hpp"
#include "shader.hpp"

// // Forward declarations to prevent circular dependencies
// class MyObject;
// class MyShader;

class MyScene {
private:
  std::unordered_map<std::string, std::shared_ptr<MyShader>> scene_shaders;
  std::unordered_map<std::string, std::shared_ptr<MyObject>> scene_objects;
  MyCamera camera;

public:
  MyScene(const MyCamera &camera);
  ~MyScene();

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

  void animateObject(std::shared_ptr<MyObject> object, float deltaTime);

  void renderScene(MyWindow &window) const;

  void handleMouseClick(int mouseX, int mouseY, const MyCamera &camera,
                        int windowWidth, int windowHeight);

  void moveCamera(MyCamera &camera, const glm::vec3 &direction) {
    camera.translate(direction);
  }

  void handleSpacePress() {
    std::cout << "Space bar pressed: Perform action (e.g., jump, interact)" << std::endl;
    // Add game logic here
  }

  void handleEnterPress() {
    std::cout << "Enter key pressed: Perform action (e.g., confirm, start game)" << std::endl;
    // Add game logic here
  }
};

#endif