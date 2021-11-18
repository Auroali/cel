#pragma once

#include "cel/tree.h"
#include <memory>
#include <filesystem>


namespace cel 
{
class object;

class scene {
    tree<std::shared_ptr<object>> objs;
public:
    void add_object(std::shared_ptr<object> object);
    void try_remove_obj(std::weak_ptr<object> obj);
    
    tree<std::shared_ptr<object>>& get_obj_tree();

    std::weak_ptr<object> get_object_by_name(const std::string& name);
    node<std::shared_ptr<object>>* get_node_by_object(std::weak_ptr<object> obj);

    void write(const std::filesystem::path& path);
    static std::shared_ptr<scene> read(const std::filesystem::path& path);
};
}