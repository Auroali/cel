#include "component.h"
#include "model_component.h"
#include "cel/framework/scene.h"

namespace cel {
    

    component::component() {
        this->trans = transform();
    }

    REFLECT_COMPONENT_DEFINE(model_component)
    REFLECT_MEMBER_WITH(model, 0)
    REFLECT_END()

    cel::render::model& model_component::get_model() {
        return model;
    }
    
    void model_component::set_model(cel::render::model&& model) {
        this->model = model;
    }

    model_component::model_component() {
        
    }
    model_component::model_component(cel::render::model&& m) {
        this->model = m;
    }
}