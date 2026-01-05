#ifndef GK_3D_COMMON_H
#define GK_3D_COMMON_H

#include <format>
#include <print>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "glad/gl.h"
#define GLFW_INCLUDE_GLU 1
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

using glm::vec3, glm::vec4, glm::mat3, glm::mat4, glm::quat;
typedef uint GLuint;

#define ERR_AND_DIE(reason) \
    do { \
        std::println(stderr, "fatal error in {}, line {}", __FILE__, __LINE__); \
        std::println(stderr, "reason: {}", (reason)); \
        exit(EXIT_FAILURE); \
    } while (0)


inline std::string slurpFile(const std::string& path)
{
    std::ifstream ifs(path);
    if (ifs.fail())
    {
        ERR_AND_DIE(std::format("can't open file `{}`", path));
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();

    return ss.str();
}

#endif //GK_3D_COMMON_H