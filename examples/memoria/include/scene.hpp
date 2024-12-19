#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include "camera.hpp"
#include "window.hpp"
#include <unordered_map>
#include <string>
#include <algorithm>

class MyScene
{
private:
    std::unordered_map<std::string, std::shared_ptr<MyObject>> scene_objects;
    MyCamera camera;

public:
    MyScene(const MyCamera &camera);
    ~MyScene();

    void addSceneObjects(const std::string &key, const std::shared_ptr<MyObject> &object)
    {
        scene_objects[key] = object;
    }

    std::unordered_map<std::string, std::shared_ptr<MyObject>> getAllSceneObjects()
    {
        return scene_objects;
    }

    std::shared_ptr<MyObject> getSceneObject(const std::string &key) const
    {
        return scene_objects.at(key);
    }

    void animateObject(std::shared_ptr<MyObject> object, float deltaTime);

    void renderScene(MyWindow &window) const;
};

#endif