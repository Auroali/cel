#pragma once
#include "cel/reflect/reflect.h"
#include "cel/render/model.h"
#include "component.h"

namespace cel {
    class model_component : public component {
        REFLECT();
        cel::render::model model;
    public:
        model_component();
        model_component(cel::render::model& m);
        void set_model(cel::render::model& model);
        cel::render::model& get_model();
    };
}