#ifndef GK_3D_CAMERA_H
#define GK_3D_CAMERA_H

#include "common.h"

class Camera
{
public:
    Camera() = default;

    Camera(vec3 position, vec3 target, vec3 up);

    mat4 view() const;

private:
    vec3 m_position, m_forward, m_up, m_left;
};

#endif //GK_3D_CAMERA_H