#pragma once
#include <vector>
#include "project_builder.h"
#include <string>

namespace cel {
    
    class project {
    private:
        template<typename T> friend class project_builder;
        static std::vector<project_builder_base*>* projects;
        std::string name;
    public:
        virtual ~project() {
            
        }
        static std::vector<project*> build_projects() {
            std::vector<project*> projects_built;
            for(size_t i = 0; i < projects->size(); ++i) {
                projects_built.push_back((*projects)[i]->build());
            }
            delete projects;
            return projects_built;
        }
        /**
         * @brief Gets the project's name
         * 
         * @return std::string the name of the project
         */
        std::string get_name() { return name; }
        /**
         * @brief Sets the project's name
         * 
         * @param name the new project name
         */
        void set_name(const std::string& name) { this->name = name; }
        /**
         * @brief Called once per frame, before fixed_update
         * 
         */
        virtual void update() {}
        /**
         * @brief Called 1/fixedDeltaTime per second
         * 
         * @note The frequency this function is called can be changed with fixedDeltaTime
         */
        virtual void fixed_update() {}
        /**
         * @brief Called when rendering, before rendering the scene
         * 
         */
        virtual void render() {}
        /**
         * @brief Called after constructing the project
         * 
         */
        virtual bool init() { return false; }
        /**
         * @brief Called during application exit, before deleting the project
         * 
         */
        virtual void shutdown() {}
    };
    //Automatically instantiates and registers project instances
    template<typename T> 
    class project_builder : public project_builder_base {
    public:
        std::string project_name;
        project_builder(const std::string& name) {
            if(!project::projects)
                project::projects = new std::vector<project_builder_base*>();
            project_name = name;
            project::projects->push_back(this);
        }
        virtual project* build() {
            project* projPtr = new T();
            projPtr->set_name(project_name);
            return projPtr;
        }
    };

}