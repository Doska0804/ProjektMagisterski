#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "SceneObject.h"
class Scene
{
	

	std::vector<SceneObject> sceneObjects;
	glm::mat4 projectionMatrix;
	glm::mat4 lightProjectionMatrix;
	glm::mat4 vpMatrix;
	glm::mat4 lightVpMatrix;
	int height;
	int width;

	glm::mat4 cameraWorldMatrix;
	glm::mat4 lightCameraWorldMatrix;

public:
	static Scene* scene;

public:
	Scene(int height, int width);
	void Input(GLFWwindow* window);
	void RenderScene();
	void ResizeWindow(GLFWwindow* window, int width, int height);
	
};

