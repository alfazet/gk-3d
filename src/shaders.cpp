#include "shaders.h"

constexpr size_t INFO_LOG_LEN = 2048;

void checkShaderCompilation(uint shader)
{
    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE)
    {
        char info_log[INFO_LOG_LEN];
        glGetShaderInfoLog(shader, INFO_LOG_LEN, nullptr, info_log);
        ERR_AND_DIE(std::format("shader compilation failed: {}", info_log));
    }
}

void checkProgramLinking(uint program)
{
    int ok;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok == GL_FALSE)
    {
        char info_log[INFO_LOG_LEN];
        glGetProgramInfoLog(program, INFO_LOG_LEN, nullptr, info_log);
        ERR_AND_DIE(std::format("shader program linking failed: {}", info_log));
    }
}

Shaders::Shaders(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
    std::string vertShaderCode = slurpFile(vertShaderPath);
    std::string fragShaderCode = slurpFile(fragShaderPath);
    const char* vertShaderCodePtr = vertShaderCode.c_str();
    const char* fragShaderCodePtr = fragShaderCode.c_str();
    uint vertShader = glCreateShader(GL_VERTEX_SHADER);
    uint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    uint id = glCreateProgram();

    glShaderSource(vertShader, 1, &vertShaderCodePtr, nullptr);
    glCompileShader(vertShader);
    checkShaderCompilation(vertShader);
    glShaderSource(fragShader, 1, &fragShaderCodePtr, nullptr);
    glCompileShader(fragShader);
    checkShaderCompilation(fragShader);
    glAttachShader(id, vertShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);
    checkProgramLinking(id);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    this->programId = id;
}

void Shaders::setUniformMat4(const std::string& name, const mat4& m) const
{
    int location = glGetUniformLocation(this->programId, name.c_str());
    if (location == -1)
    {
        ERR_AND_DIE(std::format("uniform {} not defined", name));
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}