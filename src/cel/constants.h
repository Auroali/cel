#pragma once
#include <glm/glm.hpp>

namespace cel::constants {
    // Vectors
    const glm::vec3 forward = glm::vec3(0,0,1);
    const glm::vec3 up = glm::vec3(0,1,0);
    const glm::vec3 right = glm::vec3(-1,0,0);
    // Shaders
    cel::render::shader main_shader;
    cel::render::shader quad_shader;

    void init_shaders() {
        main_shader = cel::render::shader("./shaders/main.vs", "./shaders/main.fs");
        quad_shader = cel::render::shader("./shaders/post.vs", "./shaders/post.fs");
    }
}