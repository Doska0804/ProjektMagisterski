#pragma once

#include <assimp/scene.h>

#include "SceneObject.h"

SceneObject* LoadScene(const char* path);
void LoadObject(aiNode* node, const aiScene* scene, SceneObject* parent);


