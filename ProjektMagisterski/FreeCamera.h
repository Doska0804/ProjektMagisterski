#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class FreeCamera
{
public:

    glm::vec3 Position = glm::vec3(1.0);
    glm::vec3 Front = glm::vec3(1.0);
    glm::vec3 Up = glm::vec3(1.0);
    glm::vec3 Right = glm::vec3(1.0);
    glm::vec3 WorldUp = glm::vec3(1.0);

    glm::mat4 perspectiveMatrix = glm::mat4(1.0);
    glm::mat4 viewMatrix = glm::mat4(1.0);

    float Yaw = -90;
    float Pitch = 0;

    FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    void CalculateViewMatrix();
    void SetProjectionMatrix(float height, float width, float near, float far);

    void ProcessMovement(glm::vec2 dir, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    void updateVectors();
};

