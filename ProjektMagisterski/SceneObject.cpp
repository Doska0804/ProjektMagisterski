#include "SceneObject.h"

SceneObject::~SceneObject()
{
	for (int i = 0; i < children.size(); i++)
		delete children[i];
	if (objectMesh)
		delete objectMesh;
}

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