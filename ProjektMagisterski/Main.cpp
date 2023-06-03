#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Scene.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << message << std::endl;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Scene::scene->ResizeWindow(window, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Scene::scene->ProcessMouse(xposIn, yposIn);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    GLFWwindow* window = glfwCreateWindow(mode->width/2, mode->height/2, "ProjektMagisterski", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
   
   // glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 0, 0);
    glViewport(0, 0, mode->width/2, mode->height/2);

    int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << glewGetErrorString(err) << std::endl;
    }
    
    glClearColor(0.25f, 0.25f, 0.4f, 1.0f);

    Scene scene(mode->width/2, mode->height/2, window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glDebugMessageCallback(MessageCallback, 0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);
  
    while (!glfwWindowShouldClose(window))
    {
        scene.CalculateDelta(glfwGetTime());
        scene.Input();
        scene.RenderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}
