#pragma once
#include <glm/glm.hpp>
#include "BaseShader.h"
class VarianceShader : public BaseShader
{
    unsigned int depthBuffer;

public:
    VarianceShader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath);
    ~VarianceShader();
    virtual void createTextures();
    virtual void deleteTextures();
};

