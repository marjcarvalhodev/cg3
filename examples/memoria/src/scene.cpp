#include "scene.hpp"

MyScene::MyScene(const MyCamera &camera) : camera(camera)
{
}

MyScene::~MyScene()
{
}

void MyScene::animateObject(std::shared_ptr<MyObject> object, float deltaTime)
{
    float angle = glm::radians(30.0f * deltaTime); // Rotate by 30 degrees per second
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    object->spin(rotation);
}

void MyScene::renderScene(MyWindow &window) const
{
    window.clearBuffers();

    // Step 1: Render opaque objects
    for (const auto &[key, object] : scene_objects)
    {
        if (!object->isTransparent)
        {
            object->render(camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPosition());
        }
    }

    // Step 2: Sort transparent objects based on distance to camera
    std::vector<std::shared_ptr<MyObject>> transparentObjects;
    for (const auto &[key, object] : scene_objects)
    {
        if (object->isTransparent)
        {
            transparentObjects.push_back(object);
        }
    }

    std::sort(transparentObjects.begin(), transparentObjects.end(),
              [&](const std::shared_ptr<MyObject> &a, const std::shared_ptr<MyObject> &b)
              {
                  float distA = glm::distance(camera.getPosition(), a->getPosition());
                  float distB = glm::distance(camera.getPosition(), b->getPosition());
                  return distA > distB; // Sort back to front
              });

    // Step 3: Enable blending and render transparent objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // Disable depth writes for transparency

    for (const auto &object : transparentObjects)
    {
        object->render(camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPosition());
    }

    // Restore default settings
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    window.swapBuffers();
}
