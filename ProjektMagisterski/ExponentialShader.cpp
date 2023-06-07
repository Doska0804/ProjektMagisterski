#include "ExponentialShader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"


ExponentialShader::ExponentialShader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath)
{
    loadShader(vertexPath, fragmentPath, &ID);
    loadShader(shadowVertexPath, shadowFragmentPath, &ShadowShaderID);
    createTextures();
}

ExponentialShader::~ExponentialShader()
{
    glDeleteRenderbuffers(1, &depthBuffer);
}

void ExponentialShader::createTextures() {
    glGenFramebuffers(1, &shadowFBOID);
    auto glstatus = glGetError();
    glGenTextures(1, &shadowTextureID);
    glBindTexture(GL_TEXTURE_2D, shadowTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
        Scene::scene->textureSize, Scene::scene->textureSize, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowTextureID, 0);

    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Scene::scene->textureSize, Scene::scene->textureSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete: " << fboStatus << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ExponentialShader::deleteTextures()
{
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteFramebuffers(1, &shadowFBOID);
    glDeleteTextures(1, &shadowTextureID);
}