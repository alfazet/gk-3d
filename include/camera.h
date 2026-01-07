#ifndef GK_3D_CAMERA_H
#define GK_3D_CAMERA_H

#include "common.h"

class Camera
{
public:
    Camera() = default;

    Camera(vec3 position, vec3 target, vec3 up);

    mat4 view() const;

    void handleKey(int key, float dt);

private:
    // forward and left point are opposite to where the camera is looking at
    vec3 m_originalPosition{};
    vec3 m_position{}, m_forward{}, m_up{}, m_left{};
    float m_pitch{}, m_yaw{}, m_roll{};
    float m_speed = 2.0f;
};

#endif //GK_3D_CAMERA_H