#include "scene.h"
#include "object.h"
#include <algorithm>
#include "cel/io/binary_stream.h"
#include "cel/reflect/reflect.h"
#include "components/component.h"

namespace cel {
    std::shared_ptr<scene> scene::active_scene;
    
    tree<std::shared_ptr<object>>& scene::get_obj_tree() {
        return objs;
    }
    std::weak_ptr<object> scene::get_object_by_name(const std::string& name) {
        std::vector<node<std::shared_ptr<object>>*> nodes = objs.get_sorted();
        tree<std::shared_ptr<object>>::iterator it = std::find_if(nodes.begin(), nodes.end(), 
        [&](node<std::shared_ptr<object>>* n){
            return n && n->val && n->val->name == name;
        });
        return it != nodes.end() ? (*it)->val : std::weak_ptr<object>();
        
        return std::weak_ptr<object>();
    }
    node<std::shared_ptr<object>>* scene::get_node_by_object(std::weak_ptr<object> obj) {
        if(auto lock = obj.lock()) {
            std::vector<node<std::shared_ptr<object>>*> nodes = objs.get_sorted();
            tree<std::shared_ptr<object>>::iterator it = std::find_if(nodes.begin(), nodes.end(), 
            [&](node<std::shared_ptr<object>>* n){
                return n && n->val == lock;
            });
            return it != nodes.end() ? *it : nullptr;
        }
        return nullptr;
    }
    void scene::try_remove_obj(std::weak_ptr<object> obj) {
        if(auto lock = obj.lock()) {
            node<std::shared_ptr<object>>* parent_node = get_node_by_object(lock->get_parent());
            if(parent_node) {
                parent_node->get_nodes().erase(std::find_if(parent_node->get_nodes().begin(), parent_node->get_nodes().end(), [&](node<std::shared_ptr<object>>& n){
                    return n.val && n.val == lock;
                }));
            }
            else {
                objs.get_nodes().erase(std::find_if(objs.get_nodes().begin(), objs.get_nodes().end(), [&](node<std::shared_ptr<object>>& n){
                    return n.val && n.val == lock;
                }));
            }
        }
    }
    void scene::add_object(std::shared_ptr<object> obj) {
        obj->container = shared_from_this();
        objs.push_back(node<std::shared_ptr<object>>(obj));
    }

    void scene::set_active() {
        active_scene = shared_from_this();
    }
    
    void scene::clear_active() {
        active_scene = std::shared_ptr<scene>();
    }

    std::weak_ptr<scene> scene::get_active_scene() {
        return active_scene;
    }
}