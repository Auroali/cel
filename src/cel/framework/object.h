#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

namespace cel {
    class scene;
    class component;

    struct transform {
    private:
        transform* parent;
        friend class object;
    public:
        glm::vec3 pos;
        glm::quat rot;
        glm::vec3 scale;

        /**
         * Returns the parent transform of
         * this transform object
         * 
         * @return a pointer to the parent,
         * points to itself if it has no parent
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

        /**
         * @brief Sets the euler rotation of this transform object
         * 
         * @param x the x rotation in radians
         * @param y the y rotation in radians
         * @param z the z rotation in radians
         */
        void set_rot_euler(float x, float y, float z);

        transform() : pos(0,0,0), rot(glm::vec3(0,0,0)), scale(1,1,1) {}
        transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : pos(pos), rot(rot), scale(scale) {}
        transform(glm::vec3 pos) : pos(pos), rot(glm::vec3(0,0,0)), scale(1,1,1) {}
    };
    /**
     * Object with a transform and name that
     * can be added to a scene
     */
    class object : public std::enable_shared_from_this<object> {
        std::weak_ptr<scene> container;
        std::weak_ptr<object> parent;
        std::vector<std::shared_ptr<component>> components;
        friend class scene;
    public:
        std::string name;
        transform trans;

        object();

        /**
         * @brief Gets the parent object
         * 
         * @return std::weak_ptr<object> the parent for this object
         */
        std::weak_ptr<object> get_parent();
        /**
         * @brief Sets the parent object
         * 
         * @param new_parent the object to set as the parent
         * @return true if parent was set successfully
         * @return false if parent was not set successfully
         */
        bool set_parent(std::weak_ptr<object> new_parent);
        /**
         * @brief Adds a component to the object
         * 
         * This will set the component's parent and transform parent
         * to this object and this object's transform
         * 
         * @param component the component to add
         */
        void add_component(std::shared_ptr<component> component);
        /**
         * @brief Returns a vector of weak_ptrs to all components
         * 
         */
        std::vector<std::weak_ptr<component>> get_components();
    };
}
