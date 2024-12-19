#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "window.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>

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
    return scene_objects.at(key);
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
    return scene_shaders.at(key);
  }

  void animateObject(std::shared_ptr<MyObject> object, float deltaTime);

  void renderScene(MyWindow &window) const;
};

#endif