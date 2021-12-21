#pragma once
#include "collider.h"

namespace cel {
    /**
     * A collider with a cube collision shape
     */
    class cube_collider_component : public collider_component {
    private:
        glm::vec3 size;
        REFLECT();
    public:
        /**
         * @brief Initialize a cube collider with a size of (1,1,1)
         */
        cube_collider_component() : cube_collider_component(glm::vec3(1,1,1)) {}
        /**
         * @brief Initialize a cube collider with a set size
         * 
         * @param size the size of the cube collider
         */
        cube_collider_component(glm::vec3 size) : size(size) {}
        virtual void on_attach(std::shared_ptr<object> obj) override;
    };
}