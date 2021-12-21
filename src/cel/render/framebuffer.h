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
        std::vector<texture> tex;
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
         * @brief Deletes the framebuffer and all attachments
         */
        void free();
        /**
         * @brief Gets the framebuffer's draw texture
         * 
         * @param index the texture index to use (in order of attach_texture calls)
         */
        texture& get_texture(size_t index);
        /**
         * @brief Attaches a texture to the framebuffer
         * 
         * @param tex the texture to attach
         * @param attachment the attachment to use (GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_STENCIL_ATTACHMENT, etc...)
         */
        void attach_texture(texture tex, GLenum attachment);

        void draw_buffers(std::vector<GLenum> attachments);
    };
}