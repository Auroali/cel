#pragma once
#include "cel/render/model.h"
#include <filesystem>

namespace cel::io {
class obj_importer {
public:
    static cel::render::model import_from_file(std::filesystem::path file);
    static void export_to_file(cel::render::model& m, std::filesystem::path file);
};
}