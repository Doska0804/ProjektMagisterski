#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Loader.h"
#include "Helpers.h"

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
	lightProjectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 40.0f);
	//lightCameraWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 6.0f));
	glm::vec3 lightPos = glm::vec3(5.0f, 20.0f, 10.0f);
	glm::vec3 frontVector = glm::normalize(-lightPos);
	glm::vec3 rightVector = glm::normalize(glm::cross(frontVector, glm::vec3(0.0f, 1.0f, 0.0f)));
	lightCameraWorldMatrix = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::cross(rightVector, frontVector)));
	lightVpMatrix = lightProjectionMatrix * lightCameraWorldMatrix;

	freeCam = FreeCamera(glm::vec3(0.0f,2.0f, 5.0f));
	freeCam.SetProjectionMatrix(height, width, 0.1f, 50.0f);

	rotCam = RotatingCamera(glm::vec3(0.0f, 3.0f, 6.0f));
	rotCam.SetPerspectiveMatrix(height, width, 0.1f, 50.0f);

	this->sceneObjects.push_back(LoadScene("./cube.obj"));

	Scene::scene = this;

	shader = new Shader("baseVertexShader.glsl", "baseFragmentShader.glsl", "baseShadowVertexShader.glsl", "baseShadowFragmentShader.glsl");
	debugQuad = new Shader("quadVertex.glsl", "quadFragment.glsl", nullptr, nullptr);
	vShader = new VarianceShader("baseVertexShader.glsl", "varianceFragmentShader.glsl", "baseShadowVertexShader.glsl", "varianceShadowFragmentShader.glsl");
	vCompute = new VarianceComputeShader("VarianceComputeShader.glsl");

	Helpers::createGaussianKernel5x5();

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
	//glCullFace(GL_FRONT);
	//shader->SetActiveShadow();
	//shader->setMatrix("vpMatrix", &lightVpMatrix);
	//for (int i = 0; i < sceneObjects.size(); i++)
	//{
	//	sceneObjects[i]->Draw();
	//}
	// 
	vShader->SetActiveShadow();
	vShader->setMatrix("vpMatrix", &lightVpMatrix);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->Draw();
	}
	//glCullFace(GL_BACK);
	/*shader->SetActive();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shader->shadowTextureID);
	glm::vec3 lightPos = glm::vec3(0.0f, 3.0f, 10.0f);
	//shader->setInt("shadowMap", 0);
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
	*/

	
	vCompute->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vCompute->textureId);
	glBindImageTexture(0, vCompute->textureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RG32F);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, vShader->shadowTextureID);
	glBindImageTexture(1, vShader->shadowTextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RG32F);
	vCompute->setFloatVector("gaussianKernel", Helpers::kernel, 25);
	glDispatchCompute((unsigned int)(1024/8), (unsigned int)(1024/8), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	vShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vCompute->textureId);
	glGenerateMipmap(GL_TEXTURE_2D);
	glClearColor(0.25f, 0.25f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 lightPos = glm::vec3(5.0f, 20.0f, 10.0f);
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
	{
		debugQuad->SetActive();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, vCompute->textureId);
		glClearColor(0.25f, 0.25f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		debugQuad->setInt("depthMap", 0);
		debugQuad->setFloat("near_plane", 0.1f);
		debugQuad->setFloat("far_plane", 40.0f);
		renderQuad();
	}
	
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
}

float Scene::GetDelta()
{
	return (float)deltaTime;
}

void Scene::ProcessMouse(double xposIn, double yposIn)
{

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
}

