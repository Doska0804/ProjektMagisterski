#pragma once
#include "Shader.h"
class ComputeShader :
    public Shader
{

public:
    unsigned int textureID;

    void setActive() override;
    virtual void createTextures() = 0;
    virtual void deleteTextures() = 0;
    void setFloatVector(const char* name, const float* vector, int size);
};

