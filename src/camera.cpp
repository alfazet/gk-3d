#include "camera.h"

Camera::Camera(vec3 position, vec3 target, vec3 up) : m_originalPosition(position), m_position(position)
{
    this->m_forward = glm::normalize(position - target);
    this->m_up = glm::normalize(up);
    this->m_left = glm::cross(this->m_up, this->m_forward);
}

mat4 Camera::view() const
{
    return glm::lookAt(this->m_position, this->m_position - this->m_forward, this->m_up);
}

void Camera::handleKey(int key, float dt)
{
    switch (key)
    {
    case GLFW_KEY_UP:
        this->m_position -= dt * this->m_speed * this->m_forward;
        break;
    case GLFW_KEY_DOWN:
        this->m_position += dt * this->m_speed * this->m_forward;
        break;
    case GLFW_KEY_LEFT:
        this->m_position -= dt * this->m_speed * this->m_left;
        break;
    case GLFW_KEY_RIGHT:
        this->m_position += dt * this->m_speed * this->m_left;
        break;
    case GLFW_KEY_0:
        this->m_position = this->m_originalPosition;
        this->m_pitch = this->m_yaw = this->m_roll = 0.0f;
        break;
    }
}