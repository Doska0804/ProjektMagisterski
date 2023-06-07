#pragma once
#include "Shader.h"
class DebugShader :
    public Shader
{


public:
    DebugShader(const char* vertexPath, const char* fragmentPath);
    void setActive() override;
};

