#pragma once
#include <vector>
#include <string>

namespace cel {
    /**
     * @brief Splits a string into a vector of strings based on a delimeter character
     * 
     * @param str the input string
     * @param delimeter the character to split the string with
     * 
     * @return std::vector<std::string> a vector containing the split string
     */
    inline std::vector<std::string> split(std::string str, char delimeter=' '){
        std::string t = std::string();
        std::vector<std::string> str_v;
        for(char c : str) {
            if(c != delimeter) {
                t += c;
            } else if(t != std::string() + delimeter && t != std::string()) {
                str_v.push_back(t);
                t = "";
            }
        }
        str_v.push_back(t);

        if(str_v.size() == 0) {
            str_v.push_back(std::string() + delimeter);
        }

        return str_v;
    }
}