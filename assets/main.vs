#version 450 core
layout(location=0) in vec3 aPos;
layout(location=2) in vec3 aNormal;
uniform mat4 mstack;
uniform mat4 projection_view;

out vec3 WorldPos;
out vec3 Normal;

void main() {
    gl_Position = projection_view * mstack * vec4(aPos, 1.0);
    WorldPos = vec3(mstack * vec4(aPos, 1.0));
    Normal = aNormal;
}