#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cel::render {
    class model {
        unsigned int vao;
        size_t size;
    public:
        model() {}
        model(unsigned int vao, size_t len) {
            this->vao = vao;
            this->size = len;
        }
        void render() {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, size);
            glBindVertexArray(0);
        }
    };
}