#pragma once
#include <vector>
#include "project_builder.h"
#include <string>
#include "cel/render/matrix_stack.h"

namespace cel {
    /**
     * Base class for all projects <br>
     * Will be initialized during cel_app::on_init
     * @see cel::project_builder
     */
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
         * @note Unlike fixed_update, this is only called once per frame at the start
         */
        virtual void update() {}
        /**
         * @brief Called 1/fixed_delta_time per second
         * 
         * @note The frequency this function is called can be changed with fixed_delta_time
         */
        virtual void fixed_update() {}
        /**
         * @brief Called when rendering, before rendering the scene
         * 
         * @param stack the matrix stack
         * 
         * @note Same as update(), except it's called at the end of the frame
         */
        virtual void render(cel::render::matrix_stack& stack) {}
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
    /**
     * Automatically constructs and registers project instances                                         <br>
     * To use, initialize in a static variable                                                          <br>
     * Ex:                                                                                              <br>
     * Project.h                                                                                        <br>
     *  class Project : public cel::project {                                                           <br>
     *      static cel::project_builder<Project> builder;                                               <br>
     *      ...                                                                                         <br>
     *  };                                                                                              <br>
     *                                                                                                  <br>
     * Project.cpp                                                                                      <br>
     *  cel::project_builder<Projecy> Project::builder = cel::project_builder<Project>("Project Name"); <br>
     *  ...                                                                                             <br>
     */
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
            project* proj_ptr = new T();
            proj_ptr->set_name(project_name);
            return proj_ptr;
        }
    };

}