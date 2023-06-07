#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::loadShader(const char* vertexPath, const char* fragmentPath, unsigned int* id) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "READING FILES ERROR" << std::endl;
        return;
    }
    
    buildShader(vertexCode.c_str(), fragmentCode.c_str(), id);
}
void Shader::buildShader(const char* vertex, const char* fragment, unsigned int* id) {
    unsigned int vertexShader, fragmentShader;
    int success;
    char infoLog[256];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 256, NULL, infoLog);
        std::cout << "VERTEX COMPILE ERROR:\n" << infoLog << std::endl;
        return;
    };

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 256, NULL, infoLog);
        std::cout << "FRAGMENT COMPILE ERROR:\n" << infoLog << std::endl;
        return;
    };


    *id = glCreateProgram();
    glAttachShader(*id, vertexShader);
    glAttachShader(*id, fragmentShader);
    glLinkProgram(*id);
    glGetProgramiv(*id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*id, 256, NULL, infoLog);
        std::cout << "LINKING ERROR:\n" << infoLog << std::endl;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setBool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(ActiveShaderID, name), (int)value);
}

void Shader::setInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(ActiveShaderID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(ActiveShaderID, name), value);
}

void Shader::setVector(const char* name, glm::vec3* vector)
{
    glUniform3fv(glGetUniformLocation(ActiveShaderID, name), 1, glm::value_ptr(*vector));
}

void Shader::setMatrix(const char* name, glm::mat4* matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ActiveShaderID, name), 1, GL_FALSE, glm::value_ptr(*matrix));
}