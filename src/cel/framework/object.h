#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

namespace cel
{
    
    class scene;
    class component;

    struct transform {
    private:
        transform* parent;
    public:
        glm::vec3 pos;
        glm::quat rot;
        glm::vec3 scale;

        /**
         * Returns the parent transform of
         * this transform object
         * 
         * @return a pointer to the parent,
         * points to itself if parent is null
         */
        transform* get_parent();
        /**
         * Convert this transform object into
         * a 4x4 matrix, taking into account
         * the parent transform
         * 
         * @return the final matrix
         */
        glm::mat4 get_mat4();

        void set_rot_euler(float x, float y, float z);

        transform() : pos(0,0,0), rot(glm::vec3(0,0,0)), scale(1,1,1) {}
        transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : pos(pos), rot(rot), scale(scale) {}
        transform(glm::vec3 pos) : pos(pos), rot(glm::vec3(0,0,0)), scale(1,1,1) {}
    };
    class object : public std::enable_shared_from_this<object> {
        std::weak_ptr<scene> container;
        std::weak_ptr<object> parent;
        std::vector<std::shared_ptr<component>> components;
    public:
        std::string name;
        transform trans;

        object(std::weak_ptr<scene> container);

        std::weak_ptr<object> get_parent();
        bool set_parent(std::weak_ptr<object> new_parent);
    
        void add_component(std::shared_ptr<component> component);
        std::vector<std::weak_ptr<object>> get_components();
    };
}
