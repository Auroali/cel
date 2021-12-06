#include "scene_io.h"
#include "binary_stream.h"
#include "cel/framework/components/component.h"
#include "cel/reflect/reflect.h"
#include "cel/framework/object.h"
#include "cel/cel.h"

namespace cel::io {
    void write_header(cel::io::binary_ofstream& stream) {
        // Identifier
        stream.write_str("\%CELSCENE\%");
        // Format version
        stream << int(SCENE_FORMAT_VER_MAJOR);
        stream << int(SCENE_FORMAT_VER_MINOR);
        stream << int(SCENE_FORMAT_VER_PATCH);
        // Num Nodes
        stream << size_t();
    }
    void write_type(cel::io::binary_ofstream& stream, std::shared_ptr<component> ptr, reflection::type* type) {
        stream << type->name.length();
        stream.write_str(type->name);
        stream << ptr->name.length();
        stream.write_str(ptr->name);
        // Get and write all members of the component
        // Type doesn't matter here
        for(reflection::member& m : type->members) {
            if(m.has_attrib(SERIALIZE)) {
                stream << uint8_t(0);
                stream << m.name.length();
                stream.write_str(m.name);
                stream.write(m.size);
                stream.write(m.ptr(ptr.get()), m.size);
            }
        }
        // Mark the end of member entries
        stream << uint8_t(1);
    }
    void write_nodes(cel::io::binary_ofstream& stream, node<std::shared_ptr<object>>& _node, size_t& num_nodes) {
        // Write Object Name
        stream << _node.val->name.size();
        stream.write_str(_node.val->name);

        // Write Transform
        stream << _node.val->trans;
        stream << _node.val->get_components().size();

        // Write all components
        for(std::weak_ptr<component> comp : _node.val->get_components()) {
            if(auto c = comp.lock()) {
                write_type(stream, c, cel::reflection::solver::get_from(c.get()));
            } else {
                stream << size_t(0);
                stream << size_t(0);
                stream << uint8_t(1);
            }
        }
        // Write sub nodes
        if(!_node.get_nodes().empty()) {
            stream << uint8_t(1);
            stream << _node.get_nodes().size();
            for(node<std::shared_ptr<object>>& n : _node.get_nodes()) {
                // Recursively write this node and all subnodes to disk
                write_nodes(stream, n, num_nodes);
            }
            stream << uint8_t(2);
        } else
            stream << uint8_t(0);
        ++num_nodes;
    }
    void scene_io::write(const std::shared_ptr<cel::scene> scene, const std::filesystem::path& path) {
        cel::io::binary_ofstream stream(path);
        size_t num_nodes = 0;

        // Write scene header
        write_header(stream);
        for(node<std::shared_ptr<object>>& n : scene->get_obj_tree().get_nodes()) {
            stream << uint8_t(0);
            write_nodes(stream, n, num_nodes);
        }
        stream << uint8_t(4);
        stream.seekp(30);
        stream << num_nodes;
    }
    std::shared_ptr<component> read_type(io::binary_ifstream& stream) {
        reflection::type* type = reflection::solver::get_by_str(stream.read_str(stream.read<size_t>()));
        if(!type)
            throw "Unable to get cel::reflection::type instance for component.";
        std::shared_ptr<component> comp = std::static_pointer_cast<component>(type->fact->make_shared());
        comp->name = stream.read_str(stream.read<size_t>());
        if(!comp)
            throw "An error occured whilst constructing component.";
        // Read members until an end marker is reached
        while(stream.read<uint8_t>() != 1) {
            std::string name = stream.read_str(stream.read<size_t>());
            size_t size = stream.read<size_t>();
            char* data = stream.read(size);
            auto member = type->get_member(name);
            if(member.has_value() && member.value().has_attrib(SERIALIZE)) {
                // Copy data from byte array to the component
                std::copy(data, data + size, member.value().ptr(comp.get()));
            }
            delete[] data;
        }
        return comp;
    }
    void read_node(std::shared_ptr<scene> ptr, cel::io::binary_ifstream& stream, std::vector<cel::node<std::shared_ptr<object>>*>& node_stack) {
        // Create the object node
        node<std::shared_ptr<object>> n(std::make_shared<object>());
        size_t name_len = stream.read<size_t>();
        n.val->name = stream.read_str(name_len);
        n.val->trans = stream.read<transform>();

        size_t size = stream.read<size_t>();
        for(size_t i = 0; i < size; i++) {
            n.val->add_component(read_type(stream));
        }
        
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
    std::shared_ptr<scene> scene_io::read(const std::filesystem::path& path) {
        cel::io::binary_ifstream stream(path);
        std::string header_txt = stream.read_str(10);
        
        if(header_txt == "\%CELSCENE\%") {
            if(stream.read<int>() != SCENE_FORMAT_VER_MAJOR)
                std::cerr << "ERROR: Scene format incompatible!\n";
                cel::request_exit(CEL_ERROR_FILEIO);
            if(stream.read<int>() != SCENE_FORMAT_VER_MINOR)
                std::cerr << "WARNING: Scene format version mismatch, things may not work as intended.\n";
            stream.read<int>();
            std::shared_ptr<scene> scene_ptr = std::make_shared<scene>();
            size_t num_nodes = stream.read<size_t>();
            std::vector<node<std::shared_ptr<object>>*> node_stack;
            while(stream.read<uint8_t>() != 4) {
                read_node(scene_ptr, stream, node_stack);
            }
            if(scene_ptr->get_obj_tree().get_sorted().size() != num_nodes)
                throw "Number of nodes do not match!";
            return scene_ptr;
        }
        std::cerr << "ERROR: Scene Header Invalid! Are you sure this is the right file? (At '" << std::filesystem::absolute(path) << "', expected '\%CELSCENE\%, got '" << header_txt << "')\n";
        return std::shared_ptr<scene>();
    }
}