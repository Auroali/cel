#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>

namespace cel::render {
    class shader {
    private:
        uint program;
    public:
        shader();
        shader(const std::string& vertex, const std::string& fragment);
        void set_mat4(const std::string& name, const glm::mat4& val);
        void set_int(const std::string& name, int id);
        void use();
        operator int() {
            return this->program;
        }
    };
}