#version 450 core
out vec4 FragColor;

in vec3 Norms;
void main() {
    FragColor = vec4(Norms, 1.0);
}