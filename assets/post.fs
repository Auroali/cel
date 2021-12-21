#version 450 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;

void main() {
    FragColor = texture(position, UV);
}