#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/// RAII wrapper around a GLFW window + OpenGL context.
class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    [[nodiscard]] bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;

    [[nodiscard]] GLFWwindow* getHandle() const { return m_handle; }

private:
    GLFWwindow* m_handle = nullptr;
};
