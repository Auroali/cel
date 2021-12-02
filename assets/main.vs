#version 450
layout(location=0) in vec3 aPos;

uniform mat4 mstack;

void main() {
    gl_Position = mstack * vec4(aPos, 1.0);
}