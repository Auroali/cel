#include "scene.h"
#include "object.h"
#include <algorithm>
#include "../io/binary_stream.h"

#define SCENE_VER 1000

namespace cel {
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
    objs.push_back(node<std::shared_ptr<object>>(obj));
}

void write_header(cel::io::binary_ofstream& stream) {
    // Identifier
    stream.write_str("\%CELSCENE\%");
    // Format version
    stream << int(SCENE_VER);
    // Num Nodes
    stream << size_t();
}
void write_nodes(cel::io::binary_ofstream& stream, node<std::shared_ptr<object>>& _node, size_t& num_nodes) {
    // Write Object Name
    stream << _node.val->name.size();
    stream.write_str(_node.val->name);

    // Write Transform
    stream << _node.val->trans;

    // Write sub nodes
    if(!_node.get_nodes().empty()) {
        stream << uint8_t(1);
        stream << _node.get_nodes().size();
        for(node<std::shared_ptr<object>>& n : _node.get_nodes()) {
            
            write_nodes(stream, n, num_nodes);
        }
        stream << uint8_t(2);
    } else
        stream << uint8_t(0);
    ++num_nodes;
}
void scene::write(const std::filesystem::path& path) {
    cel::io::binary_ofstream stream(path);
    size_t num_nodes = 0;

    // Write scene header
    write_header(stream);
    for(node<std::shared_ptr<object>>& n : objs.get_nodes()) {
        stream << uint8_t(0);
        write_nodes(stream, n, num_nodes);
    }
    stream << uint8_t(4);
    stream.seekp(14);
    stream << num_nodes;
}
void read_node(std::shared_ptr<scene> ptr, cel::io::binary_ifstream& stream, std::vector<cel::node<std::shared_ptr<object>>*>& node_stack) {
    // Create the object node
    node<std::shared_ptr<object>> n(std::make_shared<object>(ptr));
    size_t name_len = stream.read<size_t>();
    n.val->name = stream.read_str(name_len);
    n.val->trans = stream.read<transform>();

    if(!node_stack.empty())
        n.val->set_parent((*(node_stack.end()-1))->val);
    // Read and process the instruction byte
    uint8_t inst = stream.read<uint8_t>();
    if(inst == 1) {
        node_stack.push_back(&n);
        size_t num_nodes = stream.read<size_t>();
        for(size_t i = 0; i < num_nodes; ++i) {
            read_node(ptr, stream, node_stack);
        }
        if(stream.read<uint8_t>() == 2) {
            node_stack.pop_back();
        }
    }
    if(!node_stack.empty())
        (*(node_stack.end()-1))->push_back(n);
    else
        ptr->get_obj_tree().push_back(n);

}
std::shared_ptr<scene> scene::read(const std::filesystem::path& path) {
    cel::io::binary_ifstream stream(path);
    std::string header_txt = stream.read_str(10);
    
    if(header_txt == "\%CELSCENE\%") {
        if(stream.read<int>() != SCENE_VER)
            std::cerr << "WARNING: Scene Version Format mismatch! Things may not work as intended.\n";
        std::shared_ptr<scene> scene_ptr = std::make_shared<scene>();
        size_t num_nodes = stream.read<size_t>();
        std::vector<node<std::shared_ptr<object>>*> node_stack;
        tree<std::shared_ptr<object>> scene_objs;
        while(stream.read<uint8_t>() != 4) {
            read_node(scene_ptr, stream, node_stack);
        }
        return scene_ptr;
    }
    std::cerr << "ERROR: Scene Header Invalid! Are you sure this is the right file? (At '" << std::filesystem::absolute(path) << "', expected '\%CELSCENE\%, got '" << header_txt << "')\n";
    return std::shared_ptr<scene>();
}
}