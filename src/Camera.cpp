#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera(glm::vec3 eye, glm::vec3 viewDirection, glm::vec3 upVector)
{
    this->eye = eye;
    this->viewDirection = viewDirection;
    this->upVector = upVector;
    right = glm::normalize(glm::cross(viewDirection, upVector));
}

Camera::Camera()
{
    eye = glm::vec3(0.0f, 0.0f, 5.0f);
    viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(viewDirection, upVector));
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(eye, eye + viewDirection, upVector);
}

void Camera::mouseLook(int deltaX, int deltaY)
{
    const float sensitivity = 0.0005f;

    // Calculate yaw rotation matrix
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), -deltaX * sensitivity, upVector);

    // Apply rotations to viewDirection
    viewDirection = glm::vec3(yawRotation * glm::vec4(viewDirection, 0.0f));

    viewDirection = glm::normalize(viewDirection);

    right = glm::normalize(glm::cross(viewDirection, upVector));
}

void Camera::moveForward(float speed)
{
    eye += viewDirection * speed;
}
void Camera::moveBackward(float speed)
{
    eye -= viewDirection * speed;
}
void Camera::moveLeft(float speed)
{
    eye -= right * speed;
}
void Camera::moveRight(float speed)
{
    eye += right * speed;
}