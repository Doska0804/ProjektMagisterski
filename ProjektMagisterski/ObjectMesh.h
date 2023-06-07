#pragma once
#include <vector>
#include <glm/glm.hpp>
struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
};

struct Normal {

};

class ObjectMesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;


public:
	ObjectMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~ObjectMesh();
	void Initialize();
	void Draw(glm::mat4* worldMatrix);
};

