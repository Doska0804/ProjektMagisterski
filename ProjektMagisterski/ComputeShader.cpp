#include "ComputeShader.h"

#include <GL/glew.h>


void ComputeShader::setActive() {
    glUseProgram(ID);
}

void ComputeShader::setFloatVector(const char* name, const float* vector, int size)
{
    glUniform1fv(glGetUniformLocation(ID, name), size, vector);
}