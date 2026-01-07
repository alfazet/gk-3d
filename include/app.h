#ifndef GK_3D_APP_H
#define GK_3D_APP_H

#include "camera.h"
#include "common.h"
#include "shaders.h"

constexpr size_t WIN_WIDTH = 1920;
constexpr size_t WIN_HEIGHT = 1080;
constexpr size_t N_FACES = 4;
constexpr bool VSYNC = false;

constexpr int BOUND_KEYS[11] = {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_W, GLFW_KEY_S,
                                GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E, GLFW_KEY_0};

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

    void handleKeys();

private:
    float m_dt;
    uint m_vao, m_vboPos, m_vboColor, m_ebo;
};

#endif //GK_3D_APP_H