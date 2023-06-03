#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "SceneObject.h"
#include "Shader.h"
#include "FreeCamera.h"
#include "RotatingCamera.h"
#include "VarianceComputeShader.h"
#include "VarianceShader.h"
class Scene
{
	enum CameraMode {
		FREE_CAMERA,
		ROTATING_CAMERA
	};

	std::vector<SceneObject*> sceneObjects;
	glm::mat4 projectionMatrix = glm::mat4(1.0);
	glm::mat4 lightProjectionMatrix = glm::mat4(1.0);
	glm::mat4 vpMatrix = glm::mat4(1.0);
	glm::mat4 lightVpMatrix = glm::mat4(1.0);


	glm::mat4 cameraWorldMatrix = glm::mat4(1.0);
	glm::mat4 lightCameraWorldMatrix = glm::mat4(1.0);

	FreeCamera freeCam;
	RotatingCamera rotCam;

	double deltaTime = 0;
	double lastTime = 0;

	double lastMousePosx = 0;
	double lastMousePosy = 0;
	bool firstMouseInput = true;

	bool debug = false;

	CameraMode camMode = ROTATING_CAMERA;

	GLFWwindow* window;
public:
	static Scene* scene;
	Shader* shader;
	Shader* debugQuad;
	VarianceComputeShader* vCompute;
	VarianceShader* vShader;
	
	int height;
	int width;

public:
	Scene(int width, int height, GLFWwindow* window);
	void Input();
	void RenderScene();
	void ResizeWindow(GLFWwindow* window, int width, int height);
	void CalculateDelta(double time);
	float GetDelta();
	void ProcessMouse(double xposIn, double yposIn);
};

