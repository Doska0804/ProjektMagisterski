#include "Test.h"

#include <iostream>
#include <iomanip>

#include "Scene.h"
void Test::endTest()
{
	glfwSwapInterval(1);
	testRunning = false;
	outputFile.close();
	glfwSetWindowShouldClose(Scene::scene->window, true);
}

void Test::writeData()
{
	outputFile << std::setprecision(4);
	std::string shaderType = Scene::scene->shaderType == Scene::PCF ? "PCF" : Scene::scene->shaderType == Scene::VARIANCE ? "Variance" : "Exponential";
	outputFile << "Shader type: " << shaderType << std::endl;
	outputFile << "Texture size: " << Scene::scene->textureSize << std::endl;
	outputFile << "Mean FPS: " << frameTimes.size() / currentTestTime << std::endl;
	float mean = currentTestTime / frameTimes.size();
	outputFile << "Mean time: " << mean << std::endl;
	outputFile << "Max render time: " << maxFrameTime << std::endl;
	outputFile << "Min render time: " << minFrameTime << std::endl;
	outputFile << "Frames rendered: " << frameTimes.size() << std::endl;
	//outputFile << "Frame render times: " << std::endl;
	//for (int i = 0; i < frameTimes.size(); i++)
	//{
	//	outputFile << frameTimes[i] << ", ";
	//}
	outputFile << "\n" << std::endl;
}

void Test::StartTest()
{
	glfwSwapInterval(0);
	outputFile.open("Results.txt", std::ios::out);
	if (!outputFile.is_open())
		std::cout << "Error opening file";
	else
	{
		testRunning = true;
		Scene::scene->textureSize = 256;
		Scene::scene->shaderType = Scene::PCF;
		Scene::scene->camMode = Scene::ROTATING_CAMERA;
		frameTimes.reserve(300000);
		Scene::scene->SwapShaders();
		ignoreFrames = 800000;
		outputFile << "Test, resolution: " << Scene::scene->width << "x" << Scene::scene->height << std::endl;
	}
}

void Test::FrameRendered(float deltaTime)
{
	if (testRunning == false) return;
	if (ignoreFrames > 0)
	{
		ignoreFrames--;
		if (ignoreFrames == 0)
			Scene::scene->rotCam.ResetPosition();
		return;
	}

	currentTestTime += deltaTime;
	if (deltaTime > maxFrameTime)
		maxFrameTime = deltaTime;
	if (deltaTime < minFrameTime)
		minFrameTime = deltaTime;
	frameTimes.push_back(deltaTime);

	if (currentTestTime >= TEST_TIME)
	{
		writeData();
		frameTimes.clear();
		currentTestTime = 0;
		maxFrameTime = 0;
		minFrameTime = std::numeric_limits<float>::max();
		ignoreFrames = 1;
		if (Scene::scene->textureSize == 2048)
		{
			if (currentScene >= SCENE_COUNT - 1)
			{
				if (Scene::scene->shaderType == Scene::EXPONENTIAL)
				{
					endTest();
					return;
				}
				currentScene = 0;
				outputFile << "\n\n\n" << std::endl;
				Scene::scene->textureSize = 256;
				Scene::scene->shaderType = Scene::scene->shaderType == Scene::PCF ? Scene::VARIANCE : Scene::EXPONENTIAL;
				Scene::scene->SwapShaders();
				Scene::scene->SwapScenes(currentScene);
			}
			else
			{
				outputFile << "\nSceneSwap\n" << std::endl;
				currentScene++;
				Scene::scene->textureSize = 256;
				Scene::scene->SwapTextures();
				Scene::scene->SwapScenes(currentScene);
			}
		}
		else
		{
			Scene::scene->textureSize *= 2;
			Scene::scene->SwapTextures();
		}
		
	}


}