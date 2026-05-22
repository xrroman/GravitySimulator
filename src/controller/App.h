#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>

#include "../model/Camera.h"
#include "../model/Simulation.h"
#include "../view/Shader.h"
#include "../view/Window.h"
#include "../view/Mesh.h"
#include "../view/Trail.h"
#include "../view/TextureLoader.h"

/// Top-level application class (MVC entry point).
/// Model: Simulation, Body
/// View: Mesh, Trail, Shader, TextureLoader, Window
/// Controller: App (input → update → render loop)
class App
{
public:
    App(unsigned int width, unsigned int height, const char* title);
    ~App() = default;

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    Window m_window;

    Camera m_camera;
    float m_lastMouseX;
    float m_lastMouseY;
    bool m_firstMouse = true;

    std::unique_ptr<Shader> m_planetShader;
    std::unique_ptr<Shader> m_trailShader;

    Simulation m_simulation;

    struct BodyRenderer
    {
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Trail> trail;
        std::unique_ptr<TextureLoader> texture;
    };
    std::vector<BodyRenderer> m_renderers;

    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    bool m_showTrails = true;

    void initGL();
    void buildSolarSystem();

    void processInput();
    void update(float deltaTime);
    void render();

    // GLFW callbacks retrieve App* via glfwGetWindowUserPointer
    static void onFramebufferResize(GLFWwindow* window, int width, int height);
    static void onMouseMove(GLFWwindow* window, double x, double y);
    static void onMouseScroll(GLFWwindow* window, double dx, double dy);
    static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
};
