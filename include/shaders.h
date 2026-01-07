#ifndef GK_3D_SHADER_H
#define GK_3D_SHADER_H

#include "common.h"

constexpr size_t INFO_LOG_LEN = 2048;

class Shaders
{
public:
    uint programId{};

    Shaders() = default;

    Shaders(const std::string& vertShaderPath, const std::string& fragShaderPath);

    void setUniformMat4(const std::string& name, const mat4& m) const;
};

#endif //GK_3D_SHADER_H