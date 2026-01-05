#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inColor;

out vec3 color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
    // vertex position in model space (associated with the model): v = [pos.x, pos.y, pos.z, 1.0f]

    // in world space (associated with the global scene): model * v
    // in view space (associated with the camera): view * model * v
    // in clip space (NDC, a 2x2x2 cube centered at the origin): proj * view * model * v
    gl_Position = proj * view * model * vec4(pos, 1.0f);
    color = inColor;
}