#include "BaseShader.h"

#include <GL/glew.h>
#include "Scene.h"

BaseShader::~BaseShader()
{
    deleteTextures();
    glDeleteProgram(ShadowShaderID);
}

void BaseShader::setActiveShadow()
{
    ActiveShaderID = ShadowShaderID;
    glViewport(0, 0, Scene::scene->textureSize, Scene::scene->textureSize);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOID);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(ShadowShaderID);
}

void BaseShader::setActive()
{
    ActiveShaderID = ID;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Scene::scene->width, Scene::scene->height);
    glUseProgram(ID);
}

void BaseShader::deleteTextures()
{
    glDeleteFramebuffers(1, &shadowFBOID);
    glDeleteTextures(1, &shadowTextureID);
}