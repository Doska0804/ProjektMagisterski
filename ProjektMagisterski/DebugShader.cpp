#include "DebugShader.h"

#include <GL/glew.h>
#include "Scene.h"

DebugShader::DebugShader(const char* vertexPath, const char* fragmentPath)
{
	loadShader(vertexPath, fragmentPath, &ID);
}

void DebugShader::setActive()
{
    ActiveShaderID = ID;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Scene::scene->width, Scene::scene->height);
    glUseProgram(ID);
}