#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>


namespace cel
{
    
    class scene;

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
    class object {
        std::weak_ptr<scene> container;
        std::weak_ptr<object> parent;
    public:
        std::string name;
        transform trans;
    };
}
