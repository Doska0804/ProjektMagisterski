#pragma once

#include<glm/glm.hpp>

class RotatingCamera
{
public:
	glm::mat4 perspectiveMatrix = glm::mat4(1.0);
	glm::mat4 viewMatrix = glm::mat4(1.0);
	glm::vec3 Position = glm::vec3(1.0);
	glm::vec3 StartingPosition = glm::vec3(1.0);
private:
	
	

	void calculateViewMatrix();

public:

	RotatingCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	void Update(float delta);
	void SetPerspectiveMatrix(float height, float width, float near, float far);
	void ResetPosition();
};

