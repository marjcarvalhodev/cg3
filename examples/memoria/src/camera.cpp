#include "camera.hpp"

MyCamera::MyCamera(const glm::vec3 &pos, const glm::vec3 &tgt, const glm::vec3 &upDir, float aspect)
    : position(pos), target(tgt), up(upDir), fov(45.0f), aspectRatio(aspect), nearPlane(0.1f), farPlane(100.0f)
{
    updateViewMatrix();
    updateProjectionMatrix();
}

void MyCamera::updateViewMatrix()
{
    viewMatrix = glm::lookAt(position, target, up);
}

void MyCamera::updateProjectionMatrix()
{
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

const glm::mat4 &MyCamera::getViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4 &MyCamera::getProjectionMatrix() const
{
    return projectionMatrix;
}

void MyCamera::setPosition(const glm::vec3& newValue)
{
    position = newValue;
    updateViewMatrix();
}

void MyCamera::setTarget(const glm::vec3& newValue)
{
    target = newValue;
    updateViewMatrix();
}

void MyCamera::setAspectRatio(float newValue)
{
    aspectRatio = newValue;
    updateProjectionMatrix();
}