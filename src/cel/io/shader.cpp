#include "shader.h"
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include "file_reader.h"

const char* FALLBACK_FRAGMENT = 
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0);\n"
"}\n";
const char* FALLBACK_VERTEX =
"#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

namespace cel {
    shader::shader() {

    }
    void shader::use() {
        glUseProgram(program);
    }
    void shader::set_mat4(const std::string& name, const glm::mat4& val) {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &val[0][0]);
    }
    shader::shader(const std::string& v, const std::string& f) {
        program = UINT_MAX;
        try {
            std::string vStr = cel::io::file_reader::read_file(v);
            std::string fStr = cel::io::file_reader::read_file(f);
            const char* vStrC = vStr.c_str();
            const char* fStrC = fStr.c_str();
            uint vert, frag;
            int success;
            char infoLog[512];

            vert = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert, 1, &vStrC, NULL);
            glCompileShader(vert);
            glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(vert, 512, NULL, infoLog);
                std::cout << infoLog << std::endl;
            }


            frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(frag, 1, &fStrC, NULL);
            glCompileShader(frag);
            glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(frag, 512, NULL, infoLog);
                std::cout << infoLog << std::endl;
            }

            program = glCreateProgram();
            glAttachShader(program, vert);
            glAttachShader(program, frag);
            glLinkProgram(program);
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(program, 512, NULL, infoLog);
                std::cout << infoLog << std::endl;
            }
            glDeleteShader(frag);
            glDeleteShader(vert);
            std::cout << "Loaded shader " << v << ", " << f << " with ID " << program << std::endl;
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
        
    }
}