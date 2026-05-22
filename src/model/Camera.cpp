#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : m_position(position)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_worldUp(worldUp)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_movementSpeed(kDefaultSpeed)
    , m_mouseSensitivity(kDefaultSensitivity)
    , m_zoom(kDefaultZoom)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    const float velocity = m_movementSpeed * deltaTime;
    switch (direction)
    {
        case CameraMovement::Forward: m_position += m_front * velocity; break;
        case CameraMovement::Backward: m_position -= m_front * velocity; break;
        case CameraMovement::Left: m_position -= m_right * velocity; break;
        case CameraMovement::Right: m_position += m_right * velocity; break;
        case CameraMovement::Up: m_position.y += velocity; break;
        case CameraMovement::Down: m_position.y -= velocity; break;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    m_yaw += xOffset * m_mouseSensitivity;
    m_pitch += yOffset * m_mouseSensitivity;

    if (constrainPitch)
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
    m_zoom = std::clamp(m_zoom - yOffset, 1.0f, 45.0f);
}

void Camera::updateCameraVectors()
{
    const float yawRad = glm::radians(m_yaw);
    const float pitchRad = glm::radians(m_pitch);

    glm::vec3 front;
    front.x = std::cos(yawRad) * std::cos(pitchRad);
    front.y = std::sin(pitchRad);
    front.z = std::sin(yawRad) * std::cos(pitchRad);

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
