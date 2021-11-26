#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "shader.h"

namespace cel::render {
    class matrix_stack {
        std::vector<glm::mat4> matrices;
    public:
        void pop();
        void push();
        void translate(glm::vec3 pos);
        void rotate(glm::quat rot);
        void scale(glm::vec3 scale);
        void apply(shader& shader);
    };
}