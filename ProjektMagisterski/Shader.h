#pragma once

#include <glm/glm.hpp>

class Shader
{
   
    
public:
    unsigned int ID;
    unsigned int ActiveShaderID;
public:
    virtual ~Shader();
    void loadShader(const char* vertexPath, const char* fragmentPath, unsigned int* id);
    void buildShader(const char* vertex, const char* fragment, unsigned int* id);
    virtual void setActive() = 0;

   

    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setVector(const char* name, glm::vec3* vector);
    void setMatrix(const char* name, glm::mat4* matrix);
};

