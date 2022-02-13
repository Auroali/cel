#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace cel::render {
    /**
     * Represents a point in 3D space with UVs and Normals
     */
    struct vertex {
        /**
         * @brief The position of the vertex
         */
        glm::vec3 pos;
        /**
         * @brief The vertex's UVs
         */
        glm::vec2 uv;
        /**
         * @brief The vertex's normals
         */
        glm::vec3 normals;
    };
    class model {
        unsigned int vao;
        size_t size;
    public:
        /**
         * @brief Constructs an empty model with no render information
         */
        model() {}

        /**
         * @brief Constructs a model with the specified VAO
         * 
         * @param vao the VAO for the model to use
         * @param len the amount of vertices in the VAO
         */
        model(unsigned int vao, size_t len) {
            this->vao = vao;
            this->size = len;
        }
        
        /**
         * @brief Renders the model to the current framebuffer
         */
        void render() {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, size);
            glBindVertexArray(0);
        }
    };
}