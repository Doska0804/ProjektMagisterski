#include "Helpers.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
float* Helpers::kernel = nullptr;
void Helpers::createGaussianKernel5x5()
{
	float* kernel = new float[25];
	float sigma = 1.5;
	float doubleSigmaSquared = 2 * sigma * sigma;
	float sum = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			int x = i - 2;
			int y = j - 2;
			kernel[i * 5 + j] = exp(-(x * x + y * y) / doubleSigmaSquared) / (M_PI * doubleSigmaSquared);
			sum += kernel[i * 5 + j];
		}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			kernel[i * 5 + j] /= sum;


	Helpers::kernel = kernel;
}