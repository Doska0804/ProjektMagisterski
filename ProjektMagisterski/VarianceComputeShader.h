#pragma once

#include <glm/glm.hpp>

class VarianceComputeShader
{
public:
	unsigned int ID;
    unsigned int textureId;

    VarianceComputeShader(const char* path);

    void SetActive();

    void setFloatVector(const char* name, const float* vector, int size);
};

