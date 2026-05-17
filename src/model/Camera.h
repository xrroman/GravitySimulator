#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class CameraMovement { Forward, Backward, Left, Right, Up, Down };

/// Free-fly camera driven by keyboard and mouse.
/// Uses yaw/pitch Euler angles; pitch is clamped to avoid gimbal lock.
class Camera
{
public:
    static constexpr float kDefaultYaw = -90.0f;
    static constexpr float kDefaultPitch = 0.0f;
    static constexpr float kDefaultSpeed = 25.0f;
    static constexpr float kDefaultSensitivity = 0.1f;
    static constexpr float kDefaultZoom = 45.0f;

    [[nodiscard]] const glm::vec3& getPosition() const { return m_position; }
    [[nodiscard]] float getZoom() const { return m_zoom; }
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f),
                    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
                    float yaw = kDefaultYaw,
                    float pitch = kDefaultPitch);

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void processMouseScroll(float yOffset);

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    void updateCameraVectors();
};
