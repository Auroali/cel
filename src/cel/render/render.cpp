#include "model.h"
#include "shader.h"
#include "framebuffer.h"
#include "matrix_stack.h"
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include "cel/io/file_reader.h"
#include "renderer.h"
#include "cel/framework/object.h"
#include "cel/framework/components/model_component.h"
#include "cel/framework/components/component.h"
#include <memory>

#include "cel/logger.h"

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

/**
 * Variables for storing the render buffer
 * and post-process effect quad
 */
cel::render::framebuffer render_buffer;
cel::render::model post_quad;     

namespace cel::render {
    std::shared_ptr<cel::camera> camera;

    shader::shader() {

    }

    void shader::use() {
        glUseProgram(program);
    }

    void shader::set_mat4(const std::string& name, const glm::mat4& val) {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &val[0][0]);

    }
    void shader::set_int(const std::string& name, int id) {
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
                LOG_ERROR(infoLog);
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
                LOG_ERROR(infoLog);
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
                LOG_ERROR(infoLog);
            }
            glDeleteShader(frag);
            glDeleteShader(vert);
            LOG_INFO(fmt::format("Loaded shaders {}, {} with ID {} ", v, f, program));
        } catch (const std::ifstream::failure& e) {
            LOG_ERROR(e.what());
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
        for(auto it = matrices.begin(); it != matrices.end(); ++it) {
            full *= *it;
        }
        shader.set_mat4("mstack", full);
    }
    glm::mat4& matrix_stack::get() {
        return *(matrices.end() - 1);
    }
    framebuffer::framebuffer(int width, int height) {
        glGenFramebuffers(1, &buf);
        glBindFramebuffer(GL_FRAMEBUFFER, buf);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG_ERROR("Unable to complete framebuffer!");
            cel::request_exit(CEL_ERROR_FRAMEBUFFER);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void framebuffer::attach_texture(texture tex_, GLenum attachment) {
        glBindFramebuffer(GL_FRAMEBUFFER, buf);
        if(!tex_.is_valid()) {
            LOG_ERROR("Attempted to attach invalid texture to framebuffer!");
            cel::request_exit(CEL_ERROR_FRAMEBUFFER);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex_, 0);
        tex.push_back(tex_);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void framebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, buf);
    }
    void framebuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    texture& framebuffer::get_texture(size_t index) {
        return tex[index];
    }
    void framebuffer::free() {
        if(buf != 0 && glIsFramebuffer(buf)) {
            if(glIsRenderbuffer(rbo))
                glDeleteRenderbuffers(1, &rbo);
            for(auto& tex_ : tex) {
                if(tex_.is_valid())
                    tex_.free();
            }
            glDeleteFramebuffers(1, &buf);
        }
    }
    void render_engine::init(uint64_t flags) {
        cel::window* window = cel::window::main();
        // Setup the render framebuffer
        render_buffer = cel::render::framebuffer(window->get_width(), window->get_height());
        render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGB, GL_RGB16F), GL_COLOR_ATTACHMENT0);
        render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGB, GL_RGB16F), GL_COLOR_ATTACHMENT1);
        render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGBA, GL_RGBA), GL_COLOR_ATTACHMENT2);
        render_buffer.draw_buffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
        // Setup the post process quad
        float vertices[] = {  
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };	
        unsigned int vao;
        unsigned int vbo = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

        // Unbind vertex array
        glBindVertexArray(0);
        post_quad = model(vao, 6);

        if(flags & CEL_RENDERFLAG_2D) {
            camera = std::make_shared<camera2d>();
            LOG_ERROR("2D mode is not complete! Things may not work as intended.");
        } else
            camera = std::make_shared<camera3d>();
        if(flags & CEL_RENDERFLAG_MULTISAMPLE_AA)
            glEnable(GL_MULTISAMPLE);
    }
    
    void walk_scene_tree(cel::node<std::shared_ptr<cel::object>>& n, matrix_stack& stack) {
        stack.push();
        stack.translate(n.val->trans.pos);
        stack.rotate(n.val->trans.rot);
        stack.scale(n.val->trans.scale);
        auto comps = n.val->get_components();
        std::for_each(comps.begin(), comps.end(), [&](std::weak_ptr<cel::component> c){
            if(auto lc = c.lock()) {
                if(auto model_c = dynamic_cast<cel::model_component*>(lc.get())) {
                    stack.push();
                    stack.translate(model_c->trans.pos);
                    stack.rotate(model_c->trans.rot);
                    stack.scale(model_c->trans.scale);
                    stack.apply(cel::globals::main_shader);
                    model_c->get_model().render();
                    stack.pop();
                }
            }
        });
        for(auto& n_ : n) {
            walk_scene_tree(n_, stack);
        }
        stack.pop();
    }
    void render_engine::render_scene(std::weak_ptr<cel::scene> scene, matrix_stack& stack) {
        if(auto lock = scene.lock()) {
            auto objs = lock->get_obj_tree().get_nodes();
            for(auto obj : objs) {
                walk_scene_tree(obj, stack);
            }
        }
    }

    void render_engine::render(matrix_stack& stack, std::vector<cel::project*> projects, std::weak_ptr<cel::scene> scene) {
        glViewport(0,0,cel::window::main()->get_width(),cel::window::main()->get_height());
        if(render_buffer.get_texture(0).width() != cel::window::main()->get_width() || render_buffer.get_texture(0).height() != cel::window::main()->get_height()) {
            LOG_INFO(fmt::format("Render Buffer size doesn't match screen size! Resizing... (Expected {}x{}, got {}x{}", cel::window::main()->get_width(), cel::window::main()->get_height(), render_buffer.get_texture(0).width(), render_buffer.get_texture(0).height()));
            render_buffer.free();
            render_buffer = cel::render::framebuffer(cel::window::main()->get_width(),cel::window::main()->get_height());
            render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGB, GL_RGB16F), GL_COLOR_ATTACHMENT0);
            render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGB, GL_RGB16F), GL_COLOR_ATTACHMENT1);
            render_buffer.attach_texture(texture(cel::window::main()->get_width(), cel::window::main()->get_height(), GL_RGBA, GL_RGBA), GL_COLOR_ATTACHMENT2);
            render_buffer.draw_buffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
        }
        
        render_buffer.bind();
        glEnable(GL_DEPTH_TEST);
        cel::globals::main_shader.use();
        camera->shader_setup(cel::globals::main_shader);
        glClearColor(0.0f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        for(cel::project* p : projects) {
            p->render(stack);
        }
        render_scene(scene, stack);
        render_buffer.unbind();
        
        // Render quad
        cel::globals::quad_shader.use();
        cel::globals::quad_shader.set_int("position", 0);
        cel::globals::quad_shader.set_int("normals", 1);
        cel::globals::quad_shader.set_int("colour", 2);
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);

        set_texture(GL_TEXTURE0, render_buffer.get_texture(0));
        set_texture(GL_TEXTURE1, render_buffer.get_texture(1));
        set_texture(GL_TEXTURE2, render_buffer.get_texture(2));
        post_quad.render();

    }
    void render_engine::set_texture(GLenum texture_id, texture tex) {
        glActiveTexture(texture_id);
        glBindTexture(GL_TEXTURE_2D, tex.handle());
    }
    std::weak_ptr<cel::camera> render_engine::get_camera() {
        return camera;
    }
    void framebuffer::draw_buffers(std::vector<GLenum> attachments) {
        glDrawBuffers(attachments.size(), attachments.data());
    }
}