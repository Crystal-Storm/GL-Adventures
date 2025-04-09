#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 eye, glm::vec3 viewDirection, glm::vec3 upVector);
    Camera();
    glm::mat4 getViewMatrix();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();

private:
    glm::vec3 eye;
    glm::vec3 viewDirection;
    glm::vec3 upVector;
};

#endif