#pragma once

#include "cel/tree.h"
#include <memory>



namespace cel 
{
class object;

class scene {
    tree<std::shared_ptr<object>> objs;
public:
    tree<std::shared_ptr<object>>& get_obj_tree();

    std::weak_ptr<object> get_object_by_name(const std::string& name);
};
}