#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "cel/cel.h"
#include "texture.h"

namespace cel::render {
    class framebuffer {
        unsigned int buf;
        unsigned int rbo;
        texture tex;
        friend void cel_app::render();
    public:
        framebuffer() {}
        framebuffer(int width, int height);
        void bind();
        void unbind();
        void free();
        texture& get_texture();
    };
}