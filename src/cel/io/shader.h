#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>

namespace cel {
    class shader {
    private:
        uint program;
    public:
        shader();
        shader(const std::string& vertex, const std::string& fragment);
        void set_mat4(const std::string& name, const glm::mat4& val);
        void use();
        operator int() {
            return this->program;
        }
    };
}