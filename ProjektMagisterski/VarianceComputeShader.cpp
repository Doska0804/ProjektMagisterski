#include "VarianceComputeShader.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

VarianceComputeShader::VarianceComputeShader(const char* path)
{
    std::string computeCode;
    std::ifstream computeShaderFile;

    computeShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        computeShaderFile.open(path);
        std::stringstream cShaderStream;
        cShaderStream << computeShaderFile.rdbuf();
        computeShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "READING FILES ERROR VARIANCE COMPUTE" << std::endl;
        return;
    }

    const char* computeCodeC = computeCode.c_str();

    unsigned int shader;
    int success = false;
    char infoLog[256];

    shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &computeCodeC, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 256, NULL, infoLog);
        std::cout << "COMPUTE VARIANCE COMPILE ERROR:\n" << infoLog << std::endl;
        return;
    };

    ID = glCreateProgram();
    glAttachShader(ID, shader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 256, NULL, infoLog);
        std::cout << "COMPUTE VARIANCE LINKING ERROR:\n" << infoLog << std::endl;
        return;
    };

    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F,
        1024, 1024, 0, GL_RG, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void VarianceComputeShader::SetActive() {
    glUseProgram(ID);
}



void VarianceComputeShader::setFloatVector(const char* name, const float* vector, int size)
{
    glUniform1fv(glGetUniformLocation(ID, name), size, vector);
}

