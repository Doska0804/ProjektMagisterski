#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Scene::scene->ResizeWindow(window, width, height);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.25f, 0.25f, 0.4f, 1.0f);

    Scene scene(800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        scene.Input(window);
        scene.RenderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}
