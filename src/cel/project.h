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
        static std::vector<project*> build_projects() {
            std::vector<project*> projects_built;
            for(int i = 0; i < projects->size(); i++) {
                projects_built.push_back((*projects)[i]->build());
            }
            delete projects;
            return projects_built;
        }
        std::string get_name() { return name; }
        void set_name(const std::string& name) { this->name = name; }
        virtual void update() {}
        virtual void fixed_update() {}
        virtual void render() {}
        virtual bool init() { return false; }
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