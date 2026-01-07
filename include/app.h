#ifndef GK_3D_APP_H
#define GK_3D_APP_H

#include "camera.h"
#include "common.h"
#include "shaders.h"

constexpr size_t WIN_WIDTH = 1920;
constexpr size_t WIN_HEIGHT = 1080;
constexpr size_t N_FACES = 4;
constexpr bool VSYNC = true;

constexpr int BOUND_KEYS[4] = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT};

class App
{
public:
    int width, height;
    GLFWwindow* window;
    Shaders shaders;
    Camera camera;

    App(int argc, char** argv);

    ~App();

    void run();

    void renderImguiFrame(int fps) const;

    void handleKeys() const;

private:
    float m_dt;
    uint m_vao, m_vboPos, m_vboColor, m_ebo;
};

#endif //GK_3D_APP_H