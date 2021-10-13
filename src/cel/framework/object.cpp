#include "object.h"
#include "scene.h"
#include <algorithm>

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
    return parent * (position * rotation * scale);
}

void transform::set_rot_euler(float x, float y, float z) {
    rot = glm::quat(glm::vec3(x,y,z));
}

std::weak_ptr<object> object::get_parent() {
    return parent;
}

bool object::set_parent(std::weak_ptr<object> new_parent) {
    if(auto con_locked = container.lock()) {
        //shared_ptr to this, to prevent ovject from being deleted while swapping parent nodes
        std::shared_ptr<object> self_ptr = shared_from_this();
        
        node<std::shared_ptr<object>>* new_parent_node = con_locked->get_node_by_object(new_parent);
        if(new_parent_node) {
            //Remove the object from the scene, if present
            con_locked->try_remove_obj(self_ptr);

            //Re-add the object to the scene under the new parent
            new_parent_node->emplace_back(self_ptr);
            parent = new_parent;
            return true;
        }
    }
    return false;
}

object::object(std::weak_ptr<scene> container){
    this->trans = transform();
    this->container = container;
    this->parent = std::weak_ptr<object>();
}
}