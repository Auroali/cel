#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace cel::io {
    class file_reader {
    public:
        /**
         * @brief Reads a file from disk into an std::string
         * 
         * @param path the file to read
         * @return std::string the file's contents
         */
        static std::string read_file(const std::filesystem::path& path) {
            std::ifstream stream = std::ifstream(path);

            std::stringstream str_stream;
            
            std::string line;

            while(std::getline(stream, line)) {
                str_stream << line << '\n';
            }

            return str_stream.str();
        }
    };
}