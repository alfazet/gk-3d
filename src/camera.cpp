#include "camera.h"

Camera::Camera(vec3 position, vec3 target, vec3 up) : m_position(position),
                                                      m_forward(glm::normalize(position - target)),
                                                      m_up(glm::normalize(up)),
                                                      m_left(glm::cross(this->m_up, this->m_forward))
{
}

mat4 Camera::view() const
{
    return glm::lookAt(this->m_position, this->m_position - this->m_forward, this->m_up);
}