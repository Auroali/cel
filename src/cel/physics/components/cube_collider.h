#pragma once
#include "collider.h"

namespace cel {
    class cube_collider_component : public collider_component {
    private:
        glm::vec3 size;
        REFLECT();
    public:
        cube_collider_component() : cube_collider_component(glm::vec3(1,1,1)) {}
        cube_collider_component(glm::vec3 size) : size(size) {}
        virtual void on_attach(std::shared_ptr<object> obj) override;
    };
}