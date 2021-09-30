#pragma once
#include <vector>

namespace cel {

template<typename T>
struct node {
    node(T val) {
        this->val = val;
    }
    template<typename... _Args>
    void add(_Args&&... __args) {
        nodes.emplace_back(__args);
    }
    T& get() {
        return val;
    }
    node<T>& operator= (const T& val) {
        this->val = val;
    }

    template<typename _T>
    friend class tree;
private:
    T val;
    std::vector<node<T>> nodes;
};

template<typename T>
class tree {
private:
    std::vector<node<T>> nodes;
public:
    std::vector<node<T>*> get_nodes_ordered() {
        return get_nodes_ordered(nodes);
    }
    std::vector<node<T>*> get_nodes_ordered(std::vector<node<T>>& nodes) {
        std::vector<node<T>*> ret_val;
        for(node<T>& node : nodes) {
            ret_val.push_back(&node);
            ret_val.insert(nodes.end(), get_nodes_ordered(&node.nodes));
        }
    }
};

}