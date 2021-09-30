#pragma once
namespace cel {
    class project;
    class project_builder_base {
    public:
        virtual project* build();
    };
}