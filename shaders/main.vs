#version 450 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;
layout(location=2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;
out vec3 Norms;

void main() {
    gl_Position = projection * model * vec4(aPos, 1.0);
    Norms = aNormal;
}