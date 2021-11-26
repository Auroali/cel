#pragma once

#include "cel/tree.h"
#include <memory>
#include <filesystem>

namespace cel {
    class object;

    /**
     * Tree-based container for objects
     */
    class scene : public std::enable_shared_from_this<scene> {
        tree<std::shared_ptr<object>> objs;

        static std::shared_ptr<scene> active_scene;
    public:
        /**
         * Sets the active scene to this scene object
         */
        void set_active();
        /**
         * @brief Clears the active scene
         * 
         */
        static void clear_active();
        /**
         * @brief Get the active scene object
         * 
         * @return std::weak_ptr<scene> a weak_ptr to the scene object
         */
        static std::weak_ptr<scene> get_active_scene();

        /**
         * @brief Adds an object to the scene
         * 
         * @param object the object to add
         */
        void add_object(std::shared_ptr<object> object);
        /**
         * @brief Tries to remove the specified object from the scene
         * 
         * If the object doesn't exist, this will do nothing
         * 
         * @param obj the object to remove
         */
        void try_remove_obj(std::weak_ptr<object> obj);
        
        /**
         * @brief Get the object tree
         * 
         * @return tree<std::shared_ptr<object>>& the tree containing all objects in the scene
         */
        tree<std::shared_ptr<object>>& get_obj_tree();

        /**
         * @brief Get an object in the scene by name
         * 
         * @param name the name of the object
         * @return std::weak_ptr<object> a weak_ptr to the object, if found
         */
        std::weak_ptr<object> get_object_by_name(const std::string& name);
        /**
         * @brief Get the node from the object tree that contains obj
         * 
         * @param obj the object to get
         * @return node<std::shared_ptr<object>>* a pointer to the node that contains the object 
         */
        node<std::shared_ptr<object>>* get_node_by_object(std::weak_ptr<object> obj);

        /**
         * @brief Writes the scene to a file on disk
         * 
         * The scene is stored in a binary file, consisting
         * of a 22 byte header followed by the object tree.
         * 
         * @param path the output path of the file
         */
        void write(const std::filesystem::path& path);
        /**
         * @brief Reads the scene from a file on disk
         * 
         * @param path the file to read from
         * @return std::shared_ptr<scene> the scene created from the file
         */
        static std::shared_ptr<scene> read(const std::filesystem::path& path);
    };
}