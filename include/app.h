#ifndef GK_3D_APP_H
#define GK_3D_APP_H

#include "common.h"
#include "shaders.h"

constexpr size_t WIN_WIDTH = 1920;
constexpr size_t WIN_HEIGHT = 1080;

class App
{
public:
    int width, height;
    GLFWwindow* window;
    Shaders* shaders;

    App(int argc, char** argv);

    ~App();

    void run() const;

private:
    float m_dt = 0;
    uint m_vao, m_vboPos, m_vboColor, m_ebo;
};

#endif //GK_3D_APP_H