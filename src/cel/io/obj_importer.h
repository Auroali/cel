#pragma once
#include "cel/render/model.h"
#include <filesystem>

namespace cel::io {
    /**
     * Importer for Wavefront OBJ model files
     */    
    extern cel::render::model import_obj_model(std::filesystem::path file);
    
}