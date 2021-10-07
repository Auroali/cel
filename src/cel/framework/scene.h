#pragma once

#include "cel/tree.h"
#include <memory>



namespace cel 
{
class object;

class scene {
    tree<std::shared_ptr<object>> objs;
public:
    void add_object(std::shared_ptr<object> object);
    
    tree<std::shared_ptr<object>>& get_obj_tree();

    std::weak_ptr<object> get_object_by_name(const std::string& name);
    node<std::shared_ptr<object>>* get_node_by_object(std::weak_ptr<object> obj);
};
}