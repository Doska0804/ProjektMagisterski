#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "SceneObject.h"
#include "DebugShader.h"
#include "BaseShader.h"
#include "FreeCamera.h"
#include "RotatingCamera.h"
#include "ComputeShader.h"
#include "Test.h"

class Scene
{


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

	Test tester;

	GLFWwindow* window;
public:
	enum CameraMode {
		FREE_CAMERA,
		ROTATING_CAMERA
	};

	enum ShaderType {
		PCF,
		VARIANCE,
		EXPONENTIAL
	};

	static Scene* scene;
	DebugShader* debugQuad;

	BaseShader* shader;
	ComputeShader* cShader;
	
	int height;
	int width;
	CameraMode camMode = ROTATING_CAMERA;
	ShaderType shaderType = EXPONENTIAL;

	bool debug = false;

	int textureSize = 256;

public:
	Scene(int width, int height, GLFWwindow* window);
	void Input();
	void RenderScene();
	void ResizeWindow(GLFWwindow* window, int width, int height);
	void CalculateDelta(double time);
	float GetDelta();
	void ProcessMouse(double xposIn, double yposIn);

	void ComputeExponential();
	void ComputeVariance();
	void DrawDebug();

	void SwapTextures();
	void SwapShaders();
	void SwapScenes(int sceneID);
};

