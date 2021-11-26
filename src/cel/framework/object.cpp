#include "object.h"
#include "scene.h"
#include <algorithm>
#include "components/component.h"

namespace cel {
    transform* transform::get_parent() {
        return parent ? parent : this;
    }

    glm::mat4 transform::get_mat4() {
        glm::mat4 position = glm::translate(glm::mat4(1.0), this->pos);
        glm::mat4 rotation = glm::mat4_cast(this->rot);
        glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);
        glm::mat4 parent = glm::mat4(1.0);
        if(get_parent() != this)
            parent = get_parent()->get_mat4();
        return glm::mat4(1.0); // /**parent */ (position * rotation * scale);
    }

    void transform::set_rot_euler(float x, float y, float z) {
        rot = glm::quat(glm::vec3(x,y,z));
    }

    std::weak_ptr<object> object::get_parent() {
        return parent;
    }

    bool object::set_parent(std::weak_ptr<object> new_parent) {
        if(auto con_locked = container.lock()) {
            if(auto new_parent_obj = new_parent.lock()) {
                //shared_ptr to this, to prevent ovject from being deleted while swapping parent nodes
                std::shared_ptr<object> self_ptr = shared_from_this();
                
                node<std::shared_ptr<object>>* new_parent_node = con_locked->get_node_by_object(new_parent);
                if(new_parent_node) {
                    //Remove the object from the scene, if present
                    con_locked->try_remove_obj(self_ptr);

                    //Re-add the object to the scene under the new parent
                    new_parent_node->emplace_back(self_ptr);
                    parent = new_parent;
                    trans.parent = &new_parent_obj->trans;
                    return true;
                }
            }
        }
        return false;
    }

    object::object(){
        this->trans = transform();
        this->parent = std::weak_ptr<object>();
    }
    void object::add_component(std::shared_ptr<component> component) {
        component->parent = shared_from_this();
        component->trans.parent = &component->trans;
        components.push_back(component);
    }
    std::vector<std::weak_ptr<component>> object::get_components() {
        std::vector<std::weak_ptr<component>> comps;
        comps.reserve(components.size());
        for(std::shared_ptr<component> c : components) {
            comps.push_back(c);
        }
        return comps;
        
    }
}