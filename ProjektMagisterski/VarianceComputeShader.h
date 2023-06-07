#pragma once

#include <glm/glm.hpp>
#include "ComputeShader.h"
class VarianceComputeShader : public ComputeShader
{
public:
    VarianceComputeShader(const char* path);
    ~VarianceComputeShader();
    void createTextures() override;
    void deleteTextures() override;
};

