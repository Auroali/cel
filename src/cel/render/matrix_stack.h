#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "shader.h"

namespace cel::render {
    class matrix_stack {
        std::vector<glm::mat4> matrices;
    public:
        /**
         * Removes the last item from the stack 
         */
        void pop();

        /**
         * Pushes a new glm::mat4 to the stack
         */
        void push();

        /**
         * @brief Translates the current matrix
         * 
         * @param pos the vec3 to move the matrix by
         */
        void translate(glm::vec3 pos);
        
        /**
         * @brief Rotates the current matrix
         * 
         * @param rot the quaternion to rotate the matrix by
         */
        void rotate(glm::quat rot);

        /**
         * @brief Scales the current matrix
         * 
         * @param scale the vec3 to scale the matrix by
         */
        void scale(glm::vec3 scale);

        /**
         * @brief Applies the matrix stack to a shader
         * 
         * @param shader the shader to use
         */
        void apply(shader& shader);

        /**
         * @brief Gets the current matrix in the stack
         * 
         * @return glm::mat4& the current matrix
         */
        glm::mat4& get();
    };
}