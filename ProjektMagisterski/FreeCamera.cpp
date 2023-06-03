#include "FreeCamera.h"
#include <glm/gtc/matrix_transform.hpp>

FreeCamera::FreeCamera(glm::vec3 position, glm::vec3 up)
{
    Position = position;
    WorldUp = up;
    Up = up;
    updateVectors();
}

void FreeCamera::CalculateViewMatrix()
{
    viewMatrix = glm::lookAt(Position, Position + Front, Up);
}

void FreeCamera::SetProjectionMatrix(float height, float width, float near, float far)
{
    perspectiveMatrix = glm::perspective(glm::radians(45.0f), width / height, near, far);
}

void FreeCamera::ProcessMovement(glm::vec2 dir, float deltaTime)
{
    float velocity = 2 * deltaTime;
    if (dir.y > 0)
        Position += Front * velocity;
    else if (dir.y < 0)
        Position -= Front * velocity;
    if (dir.x < 0)
        Position -= Right * velocity;
    else if (dir.x > 0)
        Position += Right * velocity;

    CalculateViewMatrix();
}

void FreeCamera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= 0.01;
    yoffset *= 0.01;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Yaw > 360)
    {
        Yaw -= 360;
    }
    else if (Yaw < -360)
    {
        Yaw += 360;
    }


    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateVectors();
}


void FreeCamera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, glm::vec3(0.0, 1.0, 0.0)));  
    Up = glm::normalize(glm::cross(Right, Front));
    CalculateViewMatrix();
}