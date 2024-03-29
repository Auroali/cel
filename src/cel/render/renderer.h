#pragma once
#include "shader.h"
#include "model.h"
#include "framebuffer.h"
#include "texture.h"
#include "cel/framework/scene.h"
#include <memory>

#define CEL_RENDERFLAG_2D             0x01
#define CEL_RENDERFLAG_MULTISAMPLE_AA 0x02

/**
 * @namespace cel::render Contains cel rendering code
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
         * @see cel::render::render_engine::set_flags()
         * 
         * @param flags the flags to use for initialization
         * @param camera a reference to a camera pointer, will be set depending on the 2D flag
         */
        static void init(uint64_t flags);

        /**
         * @brief Sets the flags used for init()
         * 
         * @param flags the flags to use
         */
        static void set_flags(uint64_t flags);

        /**
         * @brief Gets the current camera
         * 
         * @return std::weak_ptr<cel::camera> the current camera
         */
        static std::weak_ptr<cel::camera> get_camera();

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