#pragma once

#include <glm/glm.hpp>

class Shader
{
   
    
public:
    unsigned int ID;
    unsigned int ShadowShaderID;
    unsigned int shadowTextureID;
    unsigned int shadowFBOID;
    unsigned int ActiveShaderID;
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath);
    ~Shader();

    void loadShader(const char* vertexPath, const char* fragmentPath, unsigned int* id);
    void buildShader(const char* vertex, const char* fragment, unsigned int* id);

    void SetActive();
    void SetActiveShadow();

    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setVector(const char* name, glm::vec3* vector);
    void setMatrix(const char* name, glm::mat4* matrix);
};

