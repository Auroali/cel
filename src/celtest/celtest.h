#pragma once
#include <cel/project.h>
#include <iostream>
#include <cel/project_builder.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cel/render/shader.h>
#include <cel/window.h>
#include <cel/cel.h>
#include <cel/time.h>
#include <cel/io/binary_stream.h>
#include <cel/framework/object.h>
#include <cel/framework/scene.h>
#include <cel/io/obj_importer.h>

#include <cel/reflect/reflect.h>
#include <cel/framework/components/component.h>

#include <cel/framework/components/model_component.h>

#include <memory>

#include <cel/physics/components/rigidbody.h>
#include <cel/physics/components/cube_collider.h>

#include <cel/render/renderer.h>

cel::render::model m(0,0);

class test_comp : public cel::component {
    REFLECT();
public:
    int test;
    int test2;
};

class proj : public cel::project {
public:
    static cel::project_builder<proj> builder;
    unsigned int vao = 0;
    
    virtual bool init() override {
        std::shared_ptr<cel::scene> s1 = std::make_shared<cel::scene>();
        std::shared_ptr<cel::object> obj = std::make_shared<cel::object>();
        obj->add_component(std::make_shared<cel::model_component>(cel::io::import_obj_model("./assets/newell_teaset/teapot.obj").value_or(cel::render::model())));
        s1->add_object(obj);
        obj->add_component(std::make_shared<cel::cube_collider_component>());
        obj->add_component(std::make_shared<cel::rigidbody_component>());
        s1->set_active();
        
        float vertices[] = {
            -1,-1,0,
            0,1,0,
            1,-1,0
        };
        vao = 0;
        unsigned int vbo = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        return true;
    }
    float yaw = 0;
    float pitch = 0;
    virtual void fixed_update() override {
        cel::window::main()->set_cursor_mode(cel::cursor_mode::LOCKED);
        if(auto cam = cel::render::render_engine::get_camera().lock()) {
            if(cel::window::main()->get_input_handler().key_down(cel::keyboard::S))
                cam->translate(-cam->get_forward() * (float)cel::time::fixed_delta_time);
            if(cel::window::main()->get_input_handler().key_down(cel::keyboard::W))
                cam->translate(cam->get_forward() * (float)cel::time::fixed_delta_time);
            if(cel::window::main()->get_input_handler().key_down(cel::keyboard::A))
                cam->translate(-cam->get_right() * (float) cel::time::fixed_delta_time);
            if(cel::window::main()->get_input_handler().key_down(cel::keyboard::D))
                cam->translate(cam->get_right() * (float) cel::time::fixed_delta_time);
            yaw -= cel::window::main()->get_input_handler().get_mouse_delta_x() * 10 * cel::time::fixed_delta_time;
            pitch += cel::window::main()->get_input_handler().get_mouse_delta_y() * 10 * cel::time::fixed_delta_time;
            pitch = std::clamp(pitch, -89.f, 89.f);
            cam->set_rot_euler(glm::radians(pitch),glm::radians(yaw),0);
        }
    }
    virtual void render(cel::render::matrix_stack& stack) override {
        stack.push();
        stack.rotate(glm::quat(glm::vec3(0,0,glm::radians(90.f))));
        stack.translate(glm::vec3(0,-5,0));
        stack.apply(cel::globals::main_shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        stack.pop();
        
    }
};
