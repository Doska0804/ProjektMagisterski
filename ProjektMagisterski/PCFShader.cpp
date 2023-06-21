#include "PCFShader.h"
#include <GL/glew.h>

#include "Scene.h"

PCFShader::PCFShader(const char* vertexPath, const char* fragmentPath, const char* shadowVertexPath, const char* shadowFragmentPath)
{
    loadShader(vertexPath, fragmentPath, &ID);
    loadShader(shadowVertexPath, shadowFragmentPath, &ShadowShaderID);
    createTextures();

}

void PCFShader::createTextures()
{
    glGenFramebuffers(1, &shadowFBOID);

    glGenTextures(1, &shadowTextureID);
    glBindTexture(GL_TEXTURE_2D, shadowTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        Scene::scene->textureSize, Scene::scene->textureSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

