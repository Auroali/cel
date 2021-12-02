#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "cel/cel.h"
#include "texture.h"

namespace cel::render {
    /**
     * Interface for an OpenGL framebuffer
     */
    class framebuffer {
        unsigned int buf;
        unsigned int rbo;
        texture tex;
        friend void cel_app::render();
    public:
        /**
         * @brief Constructs an empty framebuffer
         */
        framebuffer() {}
        /**
         * @brief Constructs a framebuffer for drawing with the dimensions of `width` by `height`
         * 
         * @param width the width of the framebuffer
         * @param height the height of the framebuffer
         */
        framebuffer(int width, int height);
        /**
         * @brief Binds this framebuffer as the active framebuffer
         */
        void bind();
        /**
         * @brief Unbinds the currently active framebuffer
         */
        void unbind();
        /**
         * @brief Deletes the framebuffer
         */
        void free();
        /**
         * @brief Gets the framebuffer's draw texture
         */
        texture& get_texture();
    };
}