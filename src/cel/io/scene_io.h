#pragma once
#include <filesystem>
#include "cel/framework/scene.h"
#include <memory>

#define SCENE_FORMAT_VER_MAJOR 1
#define SCENE_FORMAT_VER_MINOR 1
#define SCENE_FORMAT_VER_PATCH 0

namespace cel::io {
    class scene_io {
    public:
        /**
         * @brief Writes the scene to a file on disk
         * 
         * The scene is stored in a binary file, consisting
         * of a 22 byte header followed by the object tree.
         * 
         * @param path the output path of the file
         */
        static void write(const std::shared_ptr<cel::scene> scene, const std::filesystem::path& path);
        /**
         * @brief Reads the scene from a file on disk
         * 
         * @param path the file to read from
         * @return std::shared_ptr<scene> the scene created from the file
         */
        static std::shared_ptr<scene> read(const std::filesystem::path& path);
    };
}