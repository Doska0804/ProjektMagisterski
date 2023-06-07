#pragma once
#include "BaseShader.h"
class PCFShader :
    public BaseShader
{
public:
    PCFShader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath);
    void createTextures() override;
};

