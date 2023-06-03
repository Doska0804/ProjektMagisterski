#include "SceneObject.h"

void SceneObject::Draw()
{
	if (objectMesh)
		objectMesh->Draw(&worldMatrix);

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw();
	}
}

void SceneObject::AppendChild(SceneObject* child)
{
	children.push_back(child);
}