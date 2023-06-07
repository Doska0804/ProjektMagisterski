#pragma once
#include "Shader.h"
class BaseShader :
    public Shader
{
protected:
    unsigned int shadowFBOID;
public: 
    unsigned int ShadowShaderID;
    unsigned int shadowTextureID;

    ~BaseShader();

    void setActiveShadow();
    void setActive() override;

    virtual void deleteTextures();
    virtual void createTextures() = 0;
    
};

