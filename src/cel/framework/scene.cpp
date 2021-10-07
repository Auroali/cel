#include "scene.h"
#include "object.h"
#include <algorithm>

namespace cel {
tree<std::shared_ptr<object>>& scene::get_obj_tree() {
    return objs;
}
std::weak_ptr<object> scene::get_object_by_name(const std::string& name) {
    // TODO: TBI
    return std::weak_ptr<object>();
}
node<std::shared_ptr<object>>* scene::get_node_by_object(std::weak_ptr<object> obj) {
    // TODO: TBI
    return nullptr;
}
void scene::add_object(std::shared_ptr<object> obj) {
    objs.push_back(node<std::shared_ptr<object>>(obj));
}
}