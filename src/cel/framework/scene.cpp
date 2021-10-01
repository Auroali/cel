#include "scene.h"
#include "object.h"
#include <algorithm>

namespace cel {
tree<std::shared_ptr<object>>& scene::get_obj_tree() {
    return objs;
}
std::weak_ptr<object> scene::get_object_by_name(const std::string& name) {
    tree<std::shared_ptr<object>>::iterator begin = objs.begin(), end = objs.end();
    tree<std::shared_ptr<object>>::iterator it = std::find_if(begin, end, [&](node<std::shared_ptr<object>>* n){
        return n->val->name == name;
    });
    return it != end ? (*it)->val : nullptr;
}
}