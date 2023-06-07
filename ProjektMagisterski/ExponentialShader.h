#pragma once
#include <glm/glm.hpp>
#include "BaseShader.h"
class ExponentialShader : public BaseShader
{
    unsigned int depthBuffer;
public:
    ExponentialShader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath);
    ~ExponentialShader();
    void createTextures() override;
    void deleteTextures() override;
};

