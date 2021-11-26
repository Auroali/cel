#pragma once
#include <vector>
#include <cstddef>
#include <string>
#include <iostream>

namespace cel {
    /**
     * Wrapper class for a value of type T
     * that can contain sub-nodes
     */
    template<typename T>
    class node {
    private:
        std::vector<node<T>> nodes;
    public:
        /**
         * @brief Constructs the node with the value val
         * 
         * @param val the value of the node
         */
        node(T val) {
            this->val = val;
        }

        /**
         * @brief Pushes a new node to the back of the list
         * 
         * @param node the node to add
         * @return The node in the list
         */
        node<T>& push_back(node<T>& node) {
            nodes.push_back(node);
            return *(nodes.end()-1);
        }

        /**
         * @brief Constructs then adds a node to the back of the list
         * 
         * @param __args the constructor parameters
         * @return The constructed node
         */
        template<typename... _Args>
        node<T>& emplace_back(_Args&&... __args) {
            nodes.emplace_back(__args...);
            return *(nodes.end()-1);
        }

        /**
         * @brief Gets the list of sub-nodes
         * 
         * @return The list of nodes
         */
        std::vector<node<T>>& get_nodes() {
            return nodes;
        }

        /**
         * @brief Returns the node at the specified index
         * 
         * @param index the index of the node
         * @return The node at index
         */
        node<T>& at(size_t index) {
            return nodes[index];
        }

        /**
         * @see at()
         */
        node<T>& operator[](size_t&& index) {
            return nodes[index];
        }

        operator T() const {
            return this->val;
        }
        
        /**
         * Returns the beginning of
         * the list of nodes
         * 
         * @see get_sorted()
         * @see std::vector::begin()
         * @return An iterator at the start of the list
         */
        typename std::vector<node<T>>::iterator begin() {
            return nodes.begin();
        }

        /**
         * Returns the end of
         * the list of nodes
         * 
         * @see get_sorted()
         * @see std::vector::end()
         * @return An iterator 1 element past the end of the list
         */
        typename std::vector<node<T>>::iterator end() {
            return nodes.end();
        }

        /**
         * The value the node contains
         */
        T val;
    };

    /**
     * Class that uses nodes to represent
     * a tree structure
     */
    template<typename T>
    class tree {
        std::vector<node<T>> nodes;
        void print_nodes(node<T>& node_, const std::string& prefix) {
            std::cout << prefix << "└──" << node_.val << std::endl;
            for(node<T>& n : node_.get_nodes()) {
                print_nodes(n, prefix + "|  ");
            }
        }
    public:
        typedef typename std::vector<node<T>*>::iterator iterator;

        /**
         * @see at()
         */
        node<T>& operator[](size_t&& index) {
            return nodes[index];
        }

        tree() {
            
        }
        
        /**
         * @brief Prints the tree in a readable format
        */
        void print_tree() {
            for(node<T>& n : nodes) {
                print_nodes(n, "");
            }
        }
        /**
         * @brief Gets all the top level nodes of the tree
         * 
         * @return The top level unsorted list of nodes
         */
        std::vector<node<T>>& get_nodes() {
            return nodes;
        }
        /**
         * @brief Returns the node at the specified index
         * 
         * @param index the index of the node
         * @return The node at index
         */
        node<T>& at(size_t index) {
            return nodes[index];
        }
        /**
         * @brief Adds a node to the tree
         * 
         * @param node the node to add
         * @return The node from the nodes vector
         */
        node<T>& push_back(const node<T>& node) {
            nodes.push_back(node);
            return *(nodes.end()-1);
        }
        /**
         * @brief Constructs a node then adds it to the tree
         * 
         * @param __args constructor parameters
         * @return The node that was constructed
         */
        template<typename... _Args>
        node<T>& emplace_back(_Args&&... __args) {
            nodes.emplace_back(__args...);
            return *(nodes.end()-1);
        }
        /**
         * Recursively searches through nodes,
         * then returns a sorted list of pointers to said nodes
         * 
         * @return The sorted list of node pointers
         */
        std::vector<node<T>*> get_sorted() {
            return get_sorted(this->nodes);
        }
        
        /**
         * @see get_sorted()
        */
        std::vector<node<T>*> get_sorted(std::vector<node<T>>& nodes) {
            std::vector<node<T>*> sorted_nodes;
            for(node<T>& node_ : nodes) {
                sorted_nodes.push_back(&node_);
                std::vector<node<T>*> child_nodes = get_sorted(node_.get_nodes());
                sorted_nodes.insert(sorted_nodes.end(), child_nodes.begin(), child_nodes.end());
            }
            return sorted_nodes;
        }
    };
}