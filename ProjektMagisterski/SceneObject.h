#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ObjectMesh.h"
class SceneObject
{
private:
	glm::mat4 localMatrix = glm::mat4();
	glm::mat4 worldMatrix = glm::mat4();
	std::vector<SceneObject*> children;
public:
	ObjectMesh* objectMesh;
public:
	void Draw();
	void AppendChild(SceneObject* child);
};

