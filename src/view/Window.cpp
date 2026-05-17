#include "Window.h"

#include <iostream>
#include <cstdlib>

Window::Window(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cerr << "[Window] Failed to initialise GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_handle)
    {
        std::cerr << "[Window] Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_handle);
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_handle);
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_handle);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}
