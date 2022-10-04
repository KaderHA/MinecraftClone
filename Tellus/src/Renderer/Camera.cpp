#include "Camera.hpp"
#include "tspch.h"
#include "Input/Input.hpp"
#include "Input/KeyCodes.hpp"

namespace ts {

Camera::Camera(glm::vec3 position, glm::vec3 up, const CameraSettings& cameraSettings)
    : m_Position(position), m_Up(up), m_WorldUp(up), m_CameraSettings(cameraSettings), m_Projection(glm::mat4(1.0f)) {
    updateVectors();
}

void Camera::SetProjection(float fov, float aspect, float nearPlane, float farPlane) {
    m_Projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::OnUpdate(float dt) {
    // if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
    //     m_Speed = 100.f;
    // else
    //     m_Speed = 10.f;
    static float lastX = 0, lastY = 0;
    static bool first = true;

    float velotcity = m_CameraSettings.Speed * dt;
    if (Input::IsKeyPressed(TS_KEY_W))
        m_Position += m_Front * velotcity;
    if (Input::IsKeyPressed(TS_KEY_S))
        m_Position -= m_Front * velotcity;
    if (Input::IsKeyPressed(TS_KEY_D))
        m_Position += m_Right * velotcity;
    if (Input::IsKeyPressed(TS_KEY_A))
        m_Position -= m_Right * velotcity;
    if (Input::IsKeyPressed(TS_KEY_LEFT_CONTROL))
        m_Position -= m_WorldUp * velotcity;
    if (Input::IsKeyPressed(TS_KEY_SPACE))
        m_Position += m_WorldUp * velotcity;

    float xoffset = Input::GetMousePosition().first - lastX;
    float yoffset = lastY - Input::GetMousePosition().second;
    lastX = Input::GetMousePosition().first;
    lastY = Input::GetMousePosition().second;

    xoffset *= m_CameraSettings.Sensitivity;
    yoffset *= m_CameraSettings.Sensitivity;

    if (first && Input::GetMousePosition().first != 0) {
        first = false;
        return;
    }

    m_CameraSettings.Yaw += xoffset;
    m_CameraSettings.Pitch += yoffset;

    if (m_CameraSettings.Pitch > 89.0f)
        m_CameraSettings.Pitch = 89.0f;
    if (m_CameraSettings.Pitch < -89.0f)
        m_CameraSettings.Pitch = -89.0f;

    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_CameraSettings.Yaw)) * cos(glm::radians(m_CameraSettings.Pitch));
    direction.y = sin(glm::radians(m_CameraSettings.Pitch));
    direction.z = sin(glm::radians(m_CameraSettings.Yaw)) * cos(glm::radians(m_CameraSettings.Pitch));
    m_Front = glm::normalize(direction);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

}  // namespace ts
