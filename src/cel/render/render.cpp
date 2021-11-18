#include "model.h"
#include "shader.h"
#include "framebuffer.h"
#include "matrix_stack.h"
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include "cel/io/file_reader.h"

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

namespace cel::render {
shader::shader() {

}

void shader::use() {
    glUseProgram(program);
}

void shader::set_mat4(const std::string& name, const glm::mat4& val) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &val[0][0]);

}
void shader::set_tex(const std::string& name, int id) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), id); 
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
            glDeleteShader(vert);
            vert = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert, 1, &FALLBACK_VERTEX, NULL);
            glCompileShader(vert);
        }


        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fStrC, NULL);
        glCompileShader(frag);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            glDeleteShader(frag);
            frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(vert, 1, &FALLBACK_FRAGMENT, NULL);
            glCompileShader(vert);
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
    } catch (const std::ifstream::failure& e) {
        std::cout << e.what() << std::endl;
    }
       
}
void matrix_stack::pop() {
    matrices.pop_back();
}
void matrix_stack::push() {
    matrices.push_back(glm::mat4(1.0));
}
void matrix_stack::translate(glm::vec3 pos) {
    *(matrices.end() - 1) = glm::translate(*(matrices.end()-1), pos);
}
void matrix_stack::rotate(glm::quat rot) {
    *(matrices.end() - 1) = glm::toMat4(rot) * *(matrices.end() - 1);
}
void matrix_stack::scale(glm::vec3 scale) {
    *(matrices.end() - 1) = glm::scale(*(matrices.end()-1), scale);
}
void matrix_stack::apply(shader& shader) {
    glm::mat4 full = glm::mat4(1.0);
    for(glm::mat4 m : matrices) {
        full *= m;
    }
    shader.set_mat4("model", full);
}
framebuffer::framebuffer(int width, int height) {
    glGenFramebuffers(1, &buf);
    glBindFramebuffer(GL_FRAMEBUFFER, buf);
    tex = texture(width, height);
    if(!tex.is_valid()) {
        std::cerr << "Unable to create texture for framebuffer!\n";
        cel::request_exit(CEL_ERROR_FRAMEBUFFER);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Unable to complete framebuffer!\n";
        cel::request_exit(CEL_ERROR_FRAMEBUFFER);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, buf);
}
void framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
texture framebuffer::get_texture() {
    return tex;
}
framebuffer::~framebuffer() {
    if(glIsFramebuffer(buf)) {
        if(glIsRenderbuffer(rbo))
            glDeleteRenderbuffers(1, &rbo);
        if(tex.is_valid())
            tex.free();
        glDeleteFramebuffers(1, &buf);
    }
}
}