#include "ObjectMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

ObjectMesh::ObjectMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	Initialize();
}

ObjectMesh::~ObjectMesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void ObjectMesh::Initialize()
{
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

    glBindVertexArray(0);
}

void ObjectMesh::Draw(glm::mat4* worldMatrix)
{
    glBindVertexArray(VAO);
    Scene::scene->shader->setMatrix("model", worldMatrix);
   
    if (!Scene::scene->renderShadow)
    {
        glm::mat4 trans = glm::transpose(glm::inverse(*worldMatrix));
        Scene::scene->shader->setMatrix("tiModel", &trans);
    }
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}