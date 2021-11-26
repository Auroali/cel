#pragma once
#include "cel/render/model.h"
#include <filesystem>
#include <optional>

namespace cel::io {
    /**
     * @brief Imports a Wavefront OBJ model from disk
     * 
     * @param file the file to import
     * @return std::optional<cel::render::model> an optional containing the model, if loaded successfully
     */
    extern std::optional<cel::render::model> import_obj_model(std::filesystem::path file);
    
}