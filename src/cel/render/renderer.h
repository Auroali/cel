#pragma once
#include "shader.h"
#include "model.h"
#include "framebuffer.h"
#include "texture.h"
#include "cel/framework/scene.h"
#include <memory>

namespace cel::render {
    class project;

    class render_engine {
        static void render(cel::camera* cam, std::vector<cel::project*> projects, std::weak_ptr<cel::scene> scene);
        static void render_scene(std::weak_ptr<cel::scene> scene);
        friend void cel_app::render();
    public:
        static void init();
        static void depth_test(bool enable);
        static void stencil_test(bool enable);
    };
}