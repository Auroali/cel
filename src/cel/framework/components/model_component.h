#pragma once
#include "cel/reflect/reflect.h"
#include "cel/render/model.h"
#include "component.h"

namespace cel {
    /**
     * Component that allows attaching a model to an object
     */
    class model_component : public component {
        REFLECT();
        cel::render::model model;
    public:
        model_component();
        /**
         * @brief Construct a new model component object
         * 
         * @param m the model to use
         */
        model_component(cel::render::model&& m);
        /**
         * @brief Sets the model
         * 
         * @param model the model to use
         */
        void set_model(cel::render::model&& model);
        /**
         * @brief Gets the current model
         * 
         * @return cel::render::model& the current model
         */
        cel::render::model& get_model();
    };
}