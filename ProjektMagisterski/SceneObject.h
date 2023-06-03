#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ObjectMesh.h"
class SceneObject
{
private:
	
	
public:
	ObjectMesh* objectMesh;
	std::vector<SceneObject*> children;
	glm::mat4 localMatrix = glm::mat4(1.0f);
	glm::mat4 worldMatrix = glm::mat4(1.0f);
public:
	void Draw();
	void AppendChild(SceneObject* child);
};

