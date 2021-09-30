#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace cel::io {
    class file_reader {
    public:
        static std::string read_file(const std::string& path) {
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