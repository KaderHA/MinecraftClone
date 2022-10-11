#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ts {

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraSettings {
    float Yaw = -90.0f;  // Rotate around Y -> Look left and right
    float Pitch = 0.0f;  // Rotate around X -> Look up and down
    float Speed = 2.5f;
    float Sensitivity = 0.1f;
    float Zoom = 45.0f;
};

class Camera {
   public:
    Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), const CameraSettings& cameraSettings = CameraSettings());
    void OnUpdate(float dt);

    void SetProjection(float fov, float aspect, float nearPlane, float farPlane);
    void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
    void SetSpeed(float speed) { m_CameraSettings.Speed = speed; }
    void InversePitch();

    // Getters
    glm::mat4 GetViewProjection() const { return (m_Projection * glm::lookAt(m_Position, m_Position + m_Front, m_Up)); }
    glm::mat4 GetView() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
    glm::mat4 GetProjection() const { return m_Projection; }
    glm::vec3 GetPosition() const { return m_Position; }

   private:
    void updateVectors();

   private:
    glm::vec3 m_Position;
    glm::vec3 m_Front, m_Right, m_Up, m_WorldUp;
    glm::mat4 m_Projection;

    CameraSettings m_CameraSettings;
};

};  // namespace ts