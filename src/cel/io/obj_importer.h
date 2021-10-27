#pragma once
#include "cel/render/model.h"
#include <filesystem>

namespace cel::io {
class obj_importer {
public:
    cel::model import_from_file(std::filesystem::path file);
    void export_to_file(model& m, std::filesystem::path file);
};
}