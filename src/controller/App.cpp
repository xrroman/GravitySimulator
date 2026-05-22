#include "App.h"

#include "../model/SphereGeometry.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Unit system (G = 1): 1 distance unit = 0.1 AU, G * M_sun = 39.535
// ~31.6 real seconds = 1 simulated Earth year
static constexpr float kAU = 10.0f;
static constexpr float kMSun = 39.535f;
static constexpr float kMEarth = kMSun * 3.003e-6f;

App::App(unsigned int width, unsigned int height, const char* title)
    : m_screenWidth(width)
    , m_screenHeight(height)
    , m_window(static_cast<int>(width), static_cast<int>(height), title)
    , m_camera(glm::vec3(47.0f, 150.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -80.0f)
    , m_lastMouseX(static_cast<float>(width) * 0.5f)
    , m_lastMouseY(static_cast<float>(height) * 0.5f)
{
    GLFWwindow* win = m_window.getHandle();
    glfwSetWindowUserPointer(win, this);
    glfwSetFramebufferSizeCallback(win, onFramebufferResize);
    glfwSetCursorPosCallback(win, onMouseMove);
    glfwSetScrollCallback(win, onMouseScroll);
    glfwSetKeyCallback(win, onKey);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initGL();
    buildSolarSystem();
}

void App::initGL()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "[App] Failed to initialise GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_planetShader = std::make_unique<Shader>("shaders/planet.vert", "shaders/planet.frag");
    m_trailShader = std::make_unique<Shader>("shaders/trail.vert", "shaders/trail.frag");
}

void App::buildSolarSystem()
{
    struct BodyDef
    {
        const char* name;
        float mass;
        float radius;
        glm::vec3 position;
        glm::vec3 velocity;  // v = sqrt(G * M_sun / r), direction +Z
        glm::vec3 color;
        bool isStatic;
        const char* texturePath;
    };

    static const BodyDef kBodies[] =
    {
        { "Sun", kMSun, 1.50f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0000f}, {1.0f, 0.9f, 0.2f}, true, "assets/textures/sun.jpg" },
        { "Mercury", kMSun * 1.652e-7f, 0.30f, {kAU * 0.39f, 0.0f, 0.0f}, {0.0f, 0.0f, 3.1839f}, {0.6f, 0.6f, 0.6f}, false, "assets/textures/mercury.jpg" },
        { "Venus", kMSun * 2.448e-6f, 0.50f, {kAU * 0.72f, 0.0f, 0.0f}, {0.0f, 0.0f, 2.3433f}, {0.9f, 0.7f, 0.3f}, false, "assets/textures/venus.jpg" },
        { "Earth", kMEarth, 0.55f, {kAU * 1.00f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.9883f}, {0.2f, 0.5f, 1.0f}, false, "assets/textures/earth.jpg" },
        { "Mars", kMSun * 3.213e-7f, 0.40f, {kAU * 1.52f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.6128f}, {0.8f, 0.3f, 0.2f}, false, "assets/textures/mars.jpg" },
        { "Jupiter", kMSun * 9.545e-4f, 1.10f, {kAU * 5.20f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.8719f}, {0.8f, 0.6f, 0.4f}, false, "assets/textures/jupiter.jpg" },
        { "Saturn", kMSun * 2.858e-4f, 0.95f, {kAU * 9.50f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.6451f}, {0.9f, 0.8f, 0.5f}, false, "assets/textures/saturn.jpg" },
    };

    const auto sphereVerts = SphereGeometry::getVertices();
    const auto sphereIndices = SphereGeometry::getIndices();

    for (const auto& def : kBodies)
    {
        m_simulation.addBody(Body(
            def.name, def.mass, def.radius,
            def.position, def.velocity, def.color, def.isStatic));

        BodyRenderer renderer;
        renderer.texture = std::make_unique<TextureLoader>(def.texturePath);

        std::vector<TextureBinding> texBindings = { { renderer.texture->getID(), "texture1" } };
        renderer.mesh = std::make_unique<Mesh>(sphereVerts, sphereIndices, std::move(texBindings));
        renderer.trail = std::make_unique<Trail>();

        m_renderers.push_back(std::move(renderer));
    }
}

void App::run()
{
    while (!m_window.shouldClose())
    {
        const float now = static_cast<float>(glfwGetTime());
        m_deltaTime = now - m_lastFrame;
        m_lastFrame = now;

        processInput();
        update(m_deltaTime);
        render();

        m_window.swapBuffers();
        m_window.pollEvents();
    }
}

void App::processInput()
{
    GLFWwindow* win = m_window.getHandle();

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Forward, m_deltaTime);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Backward, m_deltaTime);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Left, m_deltaTime);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Right, m_deltaTime);
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Up, m_deltaTime);
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera.processKeyboard(CameraMovement::Down, m_deltaTime);
}

void App::update(float deltaTime)
{
    m_simulation.update(deltaTime);

    // Append trail point every 50 ms to avoid flooding the GPU buffer.
    static float s_trailAccumulator = 0.0f;
    s_trailAccumulator += deltaTime;
    if (s_trailAccumulator >= 0.05f)
    {
        s_trailAccumulator = 0.0f;
        const auto& bodies = m_simulation.getBodies();
        for (std::size_t i = 0; i < bodies.size(); ++i)
            m_renderers[i].trail->addPoint(bodies[i].position);
    }
}

void App::render()
{
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = m_camera.getViewMatrix();
    const glm::mat4 proj = glm::perspective(
        glm::radians(m_camera.getZoom()),
        static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
        0.1f, 2000.0f);

    const auto& bodies = m_simulation.getBodies();
    const glm::vec3 sunPos = bodies.empty() ? glm::vec3(0.0f) : bodies[0].position;

    m_planetShader->bind();
    m_planetShader->setMat4("view", view);
    m_planetShader->setMat4("projection", proj);
    m_planetShader->setVec3("uLightPos", sunPos);
    m_planetShader->setVec3("uViewPos", m_camera.getPosition());

    for (std::size_t i = 0; i < bodies.size(); ++i)
    {
        const Body& body = bodies[i];
        const glm::mat4 model = glm::scale(
            glm::translate(glm::mat4(1.0f), body.position),
            glm::vec3(body.radius));

        m_planetShader->setMat4("model", model);
        m_planetShader->setVec3("uColor", body.color);
        m_planetShader->setInt("uIsSun", (i == 0) ? 1 : 0);

        m_renderers[i].mesh->draw(*m_planetShader);
    }

    if (m_showTrails)
    {
        for (std::size_t i = 0; i < bodies.size(); ++i)
            m_renderers[i].trail->draw(bodies[i].color, *m_trailShader, view, proj);
    }
}

void App::onFramebufferResize(GLFWwindow* window, int width, int height)
{
    auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    app->m_screenWidth = static_cast<unsigned int>(width);
    app->m_screenHeight = static_cast<unsigned int>(height);
    glViewport(0, 0, width, height);
}

void App::onMouseMove(GLFWwindow* window, double x, double y)
{
    auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    const float xf = static_cast<float>(x);
    const float yf = static_cast<float>(y);

    if (app->m_firstMouse)
    {
        app->m_lastMouseX = xf;
        app->m_lastMouseY = yf;
        app->m_firstMouse = false;
    }

    app->m_camera.processMouseMovement(xf - app->m_lastMouseX,app->m_lastMouseY - yf);
    app->m_lastMouseX = xf;
    app->m_lastMouseY = yf;
}

void App::onMouseScroll(GLFWwindow* window, double /*dx*/, double dy)
{
    static_cast<App*>(glfwGetWindowUserPointer(window))
        ->m_camera.processMouseScroll(static_cast<float>(dy));
}

void App::onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if (action != GLFW_PRESS) return;

    switch (key)
    {
        case GLFW_KEY_T: app->m_showTrails = !app->m_showTrails; break;
        case GLFW_KEY_P: app->m_simulation.paused = !app->m_simulation.paused; break;
        case GLFW_KEY_R: app->m_simulation.reset();
                         app->m_simulation.timeScale = 1.0f;
                         app->m_renderers.clear();
                         app->buildSolarSystem();
                         break;
        case GLFW_KEY_UP: app->m_simulation.timeScale *= 2.0f; break;
        case GLFW_KEY_DOWN: app->m_simulation.timeScale /= 2.0f; break;
        default: break;
    }
}
