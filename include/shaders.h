#ifndef GK_3D_SHADER_H
#define GK_3D_SHADER_H

#include "common.h"

class Shaders
{
public:
    uint programId;

    Shaders(const std::string& vertShaderPath, const std::string& fragShaderPath);

    void setUniformMat4(const std::string& name, const mat4& m) const;
};

#endif //GK_3D_SHADER_H