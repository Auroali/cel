#pragma once
#include "shader.h"
#include "model.h"
#include "framebuffer.h"
#include "texture.h"
#include "cel/framework/scene.h"
#include <memory>

/**
* @brief Controls whether or not tbe render engine starts in 2D mode
*/
#define CEL_RENDERFLAG_2D 0x01

/**
 * @namespace cel::render Cel render 
 */
namespace cel::render {
    class project;

    class render_engine {
        static void render(matrix_stack& stack, std::vector<cel::project*> projects, std::weak_ptr<cel::scene> scene);
        static void render_scene(std::weak_ptr<cel::scene> scene, matrix_stack& stack);
        friend void cel_app::render();
    public:
        /**
         * @brief Initializes the render engine, setting up the
         * post-process quad and render buffer
         * <br>
         * <br> Behaviour can be modified by sending a CEL_SIG_RENDER_PARAMS signal
         * 
         * @see cel::send_signal
         * 
         * @param flags the flags to use for initialization
         * @param camera a reference to a camera pointer, will be set depending on the 2D flag
         */
        static void init(uint64_t flags, std::shared_ptr<cel::camera>& camera);
        /**
         * @brief Controls whether or not depth testing is enabled
         * 
         * @param enable whether or not depth testing should be enabled
         */
        static void depth_test(bool enable);
        /**
         * @brief Controls whether or not stencil testing is enabled
         * 
         * @param enable whether or not stencil testing should be enabled
         */
        static void stencil_test(bool enable);
        /**
         * @brief Sets the active texture and binds tex to it
         * 
         * @param texture_id the active texture to use
         * @param tex the texture to bind
         */
        static void set_texture(GLenum texture_id, texture tex);
    };
}