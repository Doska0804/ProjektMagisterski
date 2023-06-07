#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Loader.h"
#include "Helpers.h"

#include "VarianceComputeShader.h"
#include "VarianceShader.h"
#include "ExponentialComputeShader.h"
#include "ExponentialShader.h"
#include "PCFShader.h"

#include <iostream>

Scene* Scene::scene = nullptr;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Scene::Scene(int width, int height, GLFWwindow* window)
{
	this->height = height;
	this->width = width;
	this->window = window;
	lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 40.0f);
	//lightCameraWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 6.0f));
	glm::vec3 lightPos = glm::vec3(2.5f, 10.0f, 5.0f);
	glm::vec3 frontVector = glm::normalize(-lightPos);
	glm::vec3 rightVector = glm::normalize(glm::cross(frontVector, glm::vec3(0.0f, 1.0f, 0.0f)));
	lightCameraWorldMatrix = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::cross(rightVector, frontVector)));
	lightVpMatrix = lightProjectionMatrix * lightCameraWorldMatrix;

	freeCam = FreeCamera(glm::vec3(0.0f,2.0f, 5.0f));
	freeCam.SetProjectionMatrix(height, width, 0.1f, 50.0f);

	rotCam = RotatingCamera(glm::vec3(0.0f, 3.0f, 6.0f));
	rotCam.SetPerspectiveMatrix(height, width, 0.1f, 50.0f);

	this->sceneObjects.push_back(LoadScene("./stairs.obj"));

	Scene::scene = this;

	debugQuad = new DebugShader("quadVertex.glsl", "quadFragment.glsl");

	if (shaderType == PCF)
	{
		shader = new PCFShader("baseVertexShader.glsl", "baseFragmentShader.glsl", "baseShadowVertexShader.glsl", "baseShadowFragmentShader.glsl");
		cShader = nullptr;
	}
	else if (shaderType == VARIANCE)
	{
		shader = new VarianceShader("baseVertexShader.glsl", "varianceFragmentShader.glsl", "baseShadowVertexShader.glsl", "varianceShadowFragmentShader.glsl");
		cShader = new VarianceComputeShader("VarianceComputeShader.glsl");
	}
	else if (shaderType == EXPONENTIAL)
	{
		shader = new ExponentialShader("baseVertexShader.glsl", "exponentialFragmentShader.glsl", "baseShadowVertexShader.glsl", "exponentialShadowFragmentShader.glsl");
		cShader = new ExponentialComputeShader("exponentialComputeShader.glsl");
	}

	Helpers::createGaussianKernel5x5();

	//tester.StartTest();
}	

void RotateObjects(std::vector<SceneObject*> objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		RotateObjects((*objects[i]).children);
		objects[i]->worldMatrix = glm::rotate(objects[i]->worldMatrix, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
void Scene::RenderScene() {

	shader->setActiveShadow();
	shader->setMatrix("vpMatrix", &lightVpMatrix);
	if (shaderType == EXPONENTIAL || shaderType == VARIANCE)
	{
		glClearColor(shaderType == EXPONENTIAL ? exp(80.0f) : 1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
	}
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Draw();
	}

	if (shaderType == EXPONENTIAL)
		ComputeExponential();
	else if (shaderType == VARIANCE)
		ComputeVariance();


	glClearColor(0.25f, 0.25f, 0.4f, 1.0f);
	shader->setActive();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	if (shaderType == PCF)
	{
		glBindTexture(GL_TEXTURE_2D, shader->shadowTextureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, cShader->textureID);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glm::vec3 lightPos = glm::vec3(5.0f, 20.0f, 10.0f);
	shader->setInt("shadowMap", 0);
	shader->setVector("lightPos", &lightPos);
	shader->setMatrix("lightVPMatrix", &lightVpMatrix);
	if (camMode == FREE_CAMERA)
	{
		shader->setMatrix("view", &freeCam.viewMatrix);
		shader->setMatrix("projection", &freeCam.perspectiveMatrix);
	}
	else if (camMode == ROTATING_CAMERA)
	{
		rotCam.Update(deltaTime);
		shader->setMatrix("view", &rotCam.viewMatrix);
		shader->setMatrix("projection", &rotCam.perspectiveMatrix);

	}


	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Draw();
	}

	
	if (debug)
		DrawDebug();

}

void Scene::ResizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	this->height = height;
	this->width = width;
}

void Scene::CalculateDelta(double time)
{
	deltaTime = time - lastTime;
	lastTime = time;
	tester.FrameRendered(deltaTime);
}

float Scene::GetDelta()
{
	return (float)deltaTime;
}

void Scene::ProcessMouse(double xposIn, double yposIn)
{
	if (tester.testRunning) return;
	if (camMode == FREE_CAMERA)
	{
		if (firstMouseInput)
		{
			lastMousePosx = xposIn;
			lastMousePosy = yposIn;
			firstMouseInput = false;
		}

		float xoffset = xposIn - lastMousePosx;
		float yoffset = lastMousePosy - yposIn;

		lastMousePosx = xposIn;
		lastMousePosy = yposIn;

		freeCam.ProcessMouseMovement(xoffset, yoffset);
	}
}


void Scene::Input()
{
	if (tester.testRunning) return;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		if (camMode == FREE_CAMERA)
			camMode = ROTATING_CAMERA;
		else
			camMode = FREE_CAMERA;
	}

	if (camMode == FREE_CAMERA)
	{
		glm::vec2 dir = glm::vec2(0.0);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			dir.y = 1;
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			dir.y = -1;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			dir.x = -1;
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			dir.x = 1;

		freeCam.ProcessMovement(dir, deltaTime);
	}

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		textureSize = 256;
		SwapTextures();
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		textureSize = 512;
		SwapTextures();
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		textureSize = 1024;
		SwapTextures();
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		textureSize = 2048;
		SwapTextures();
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		shaderType = PCF;
		SwapShaders();
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		shaderType = VARIANCE;
		SwapShaders();
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		shaderType = EXPONENTIAL;
		SwapShaders();
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		shaderType = PCF;
		SwapShaders();
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		shaderType = VARIANCE;
		SwapShaders();
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		shaderType = EXPONENTIAL;
		SwapShaders();
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		SwapScenes(0);
	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		SwapScenes(1);
	}
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		SwapScenes(2);
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		debug = !debug;
}

void Scene::ComputeExponential()
{
	cShader->setActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cShader->textureID);
	glBindImageTexture(0, cShader->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shader->shadowTextureID);
	glBindImageTexture(1, shader->shadowTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	cShader->setFloatVector("gaussianKernel", Helpers::kernel, 25);
	glDispatchCompute((unsigned int)(textureSize / 8), (unsigned int)(textureSize / 8), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Scene::ComputeVariance()
{
	cShader->setActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cShader->textureID);
	glBindImageTexture(0, cShader->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shader->shadowTextureID);
	glBindImageTexture(1, shader->shadowTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);
	cShader->setFloatVector("gaussianKernel", Helpers::kernel, 25);
	glDispatchCompute((unsigned int)(textureSize / 8), (unsigned int)(textureSize / 8), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Scene::DrawDebug()
{
	debugQuad->setActive();
	glActiveTexture(GL_TEXTURE0);
	if (shaderType == EXPONENTIAL || shaderType == VARIANCE)
	{
		glBindTexture(GL_TEXTURE_2D, cShader->textureID);
		glGenerateMipmap(GL_TEXTURE_2D);
		
	}
	else
		glBindTexture(GL_TEXTURE_2D, shader->shadowTextureID);
	
	glClearColor(0.25f, 0.25f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debugQuad->setInt("depthMap", 0);
	debugQuad->setFloat("near_plane", 0.1f);
	debugQuad->setFloat("far_plane", 40.0f);
	debugQuad->setInt("isExponential", shaderType == EXPONENTIAL ? 1 : 0);
	renderQuad();
}

void Scene::SwapTextures()
{
	shader->deleteTextures();
	shader->createTextures();
	if (cShader)
	{
		cShader->deleteTextures();
		cShader->createTextures();
	}
}

void Scene::SwapShaders()
{
	delete shader;
	delete cShader;
	if (shaderType == PCF)
	{
		shader = new PCFShader("baseVertexShader.glsl", "baseFragmentShader.glsl", "baseShadowVertexShader.glsl", "baseShadowFragmentShader.glsl");
		cShader = nullptr;
	}
	else if (shaderType == VARIANCE)
	{
		shader = new VarianceShader("baseVertexShader.glsl", "varianceFragmentShader.glsl", "baseShadowVertexShader.glsl", "varianceShadowFragmentShader.glsl");
		cShader = new VarianceComputeShader("VarianceComputeShader.glsl");
	}
	else if (shaderType == EXPONENTIAL)
	{
		shader = new ExponentialShader("baseVertexShader.glsl", "exponentialFragmentShader.glsl", "baseShadowVertexShader.glsl", "exponentialShadowFragmentShader.glsl");
		cShader = new ExponentialComputeShader("exponentialComputeShader.glsl");
	}
}

void Scene::SwapScenes(int sceneID)
{
	for (int i = 0; i < sceneObjects.size(); i++)
		delete sceneObjects[i];
	sceneObjects.clear();

	std::string scenes[] = { "./stairs.obj", "./columns.obj", "./house2.obj" };

	this->sceneObjects.push_back(LoadScene(scenes[sceneID].c_str()));

}