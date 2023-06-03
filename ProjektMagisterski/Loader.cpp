#include "Loader.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>

SceneObject* LoadScene(const char* path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "LoadingError: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    SceneObject* loadedObject = new SceneObject();

    LoadObject(scene->mRootNode, scene, loadedObject);
    return loadedObject;
}

void LoadObject(aiNode* node, const aiScene* scene, SceneObject* parent)
{
    SceneObject* child = new SceneObject();
    parent->AppendChild(child);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        SceneObject* newObject = new SceneObject();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; 

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.pos = vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.norm = vector;
            }
            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        newObject->objectMesh = new ObjectMesh(vertices, indices);
        child->AppendChild(newObject);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        LoadObject(node->mChildren[i], scene, child);
    }
}