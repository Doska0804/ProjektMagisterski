#include "RotatingCamera.h"
#include <glm/gtc/matrix_transform.hpp>




RotatingCamera::RotatingCamera(glm::vec3 position) {
    Position = position;
    StartingPosition = position;
}

void RotatingCamera::Update(float delta) {
    float cosine = cos(glm::radians(30 * delta));
    float sine = sin(glm::radians(30 * delta));
    Position = glm::vec3(Position.x * cosine - Position.z * sine, Position.y, Position.x*sine + Position.z*cosine);
    calculateViewMatrix();
}

void RotatingCamera::SetPerspectiveMatrix(float height, float width, float near, float far) {
    perspectiveMatrix = glm::perspective(glm::radians(45.0f), width / height, near, far);
}

void RotatingCamera::calculateViewMatrix() {
    glm::vec3 frontVector = glm::normalize(-Position);
    glm::vec3 rightVector = glm::normalize(glm::cross(frontVector, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    viewMatrix = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::cross(rightVector, frontVector)));
}

void RotatingCamera::ResetPosition()
{
    Position = StartingPosition;
}