#pragma once

#define TEST_TIME 30.0f
#define SCENE_COUNT 3

#include <limits>
#include <fstream>
#include <vector>

class Test
{
	float maxFrameTime = 0;
	float minFrameTime = std::numeric_limits<float>::max();
	float currentTestTime = 0;
	std::vector<float> frameTimes;
	
	int currentScene = 0;

	bool ignoreFrame = true;

	std::ofstream outputFile;

	void endTest();
	void writeData();

public:
	bool testRunning = false;

	void StartTest();
	void FrameRendered(float deltaTime);

};

