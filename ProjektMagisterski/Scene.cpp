#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(int height, int width)
{
	this->height = height;
	this->width = width;

	projectionMatrix = glm::perspective(45.0f, (float)(width / height), 1.0f, 50.0f);
	lightProjectionMatrix = glm::ortho(10.0f, 10.0f, 10.0f, 10.0f, 1.0f, 50.0f);

	lightCameraWorldMatrix = glm::translate(glm::mat4(), glm::vec3(5.0f, 5.0f, 5.0f));
	cameraWorldMatrix = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 5.0f, 5.0f) + glm::vec3(0.0f, -5.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	lightVpMatrix = lightCameraWorldMatrix * lightProjectionMatrix;
	vpMatrix = cameraWorldMatrix * projectionMatrix;

	Scene::scene = this;
}



void Scene::Input(GLFWwindow* window)
{
}

void Scene::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Scene::ResizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	this->height = height;
	this->width = width;
	projectionMatrix = glm::perspective(45.0f, (float)(width / height), 1.0f, 50.0f);
}



