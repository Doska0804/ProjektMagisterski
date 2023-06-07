#pragma once
#include "ComputeShader.h"
class ExponentialComputeShader : public ComputeShader 
{
public:

    ExponentialComputeShader(const char* path);
    ~ExponentialComputeShader();
    void createTextures() override;
    void deleteTextures() override;

};

