#pragma once
#include <glm/glm.hpp>
#if ENABLE_RENDERER
#include "render/shader.h"
#endif
/**
 * @namespace cel::globals Global variables such as constants and shaders
 */
namespace cel::globals {
    // Vectors
    const glm::vec3 forward = glm::vec3(0,0,1);
    const glm::vec3 up = glm::vec3(0,1,0);
    const glm::vec3 right = glm::vec3(-1,0,0);

    // Shaders
    extern cel::render::shader main_shader;
    extern cel::render::shader quad_shader;
    extern cel::render::shader basic_shader;
    extern void init_shaders();
}