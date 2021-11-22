#version 450
out vec4 FragColor;

in vec2 UV;

uniform sampler2D sceneTex;

void main() {
    FragColor = texture(sceneTex, UV);
}