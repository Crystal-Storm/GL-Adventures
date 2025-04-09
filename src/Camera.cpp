#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 viewDirection, glm::vec3 upVector)
{
    this->eye = eye;
    this->viewDirection = viewDirection;
    this->upVector = upVector;
}

Camera::Camera()
{
    eye = glm::vec3(0.0f, 0.0f, 1.0f);
    viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(eye, viewDirection, upVector);
}

void Camera::moveForward() {}
void Camera::moveBackward() {}
void Camera::moveLeft() {}
void Camera::moveRight() {}