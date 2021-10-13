#include "object.h"
#include "scene.h"

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

bool object::set_parent(std::weak_ptr<object> new_parent) {
    if(auto con_locked = container.lock()) {
        std::vector<node<std::shared_ptr<object>>*> sorted_nodes = con_locked->get_obj_tree().get_sorted();
        for(auto n : sorted_nodes) {
            if(auto new_parent_locked = new_parent.lock()) {
                if(n->val == new_parent_locked)
                    std::cout << "Hello!" << std::endl;
            }
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