#pragma once

#include <assimp/scene.h>

#include "SceneObject.h"
class Loader
{
	static SceneObject* LoadScene(char* path);
	static void LoadObject(aiNode* node, const aiScene* scene, SceneObject* parent);
};

