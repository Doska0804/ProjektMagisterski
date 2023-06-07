#include "ExponentialComputeShader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"

ExponentialComputeShader::ExponentialComputeShader(const char* path)
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
        std::cout << "READING FILES ERROR EXPONENTIAL COMPUTE" << std::endl;
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
        std::cout << "COMPUTE EXPONENTIAL COMPILE ERROR:\n" << infoLog << std::endl;
        return;
    };

    ID = glCreateProgram();
    glAttachShader(ID, shader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 256, NULL, infoLog);
        std::cout << "COMPUTE EXPONENTIAL LINKING ERROR:\n" << infoLog << std::endl;
        return;
    };

    createTextures();

}

ExponentialComputeShader::~ExponentialComputeShader()
{
    deleteTextures();
}

void ExponentialComputeShader::createTextures()
{
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
        Scene::scene->textureSize, Scene::scene->textureSize, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ExponentialComputeShader::deleteTextures()
{
    glDeleteTextures(1, &textureID);
}