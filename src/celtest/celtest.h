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
        obj->add_component(std::make_shared<cel::model_component>(cel::io::import_obj_model("./assets/utah_teapot.obj").value_or(cel::render::model())));
        s1->add_object(obj);
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
        cel::camera* cam;
        cel::send_signal(CEL_CAM_REQ, &cam);
        if(cel::window::main()->get_input_handler().key_down(cel::keyboard::S))
            cam->translate(-cam->get_forward() * (float)cel::time::fixedDeltaTime);
        if(cel::window::main()->get_input_handler().key_down(cel::keyboard::W))
            cam->translate(cam->get_forward() * (float)cel::time::fixedDeltaTime);
        if(cel::window::main()->get_input_handler().key_down(cel::keyboard::A))
            cam->translate(-cam->get_right() * (float) cel::time::fixedDeltaTime);
        if(cel::window::main()->get_input_handler().key_down(cel::keyboard::D))
            cam->translate(cam->get_right() * (float) cel::time::fixedDeltaTime);
        yaw -= cel::window::main()->get_input_handler().get_mouse_delta_x() * 10 * cel::time::fixedDeltaTime;
        pitch += cel::window::main()->get_input_handler().get_mouse_delta_y() * 10 * cel::time::fixedDeltaTime;
        pitch = std::clamp(pitch, -89.f, 89.f);
        cam->set_rot_euler(glm::radians(pitch),glm::radians(yaw),0);
    }
    virtual void render() override {
        cel::globals::main_shader.set_mat4("model", glm::mat4(1.0));
        //m.render();
        glm::mat4 model_ground = glm::mat4(1.0);
        model_ground = glm::translate(model_ground, glm::vec3(0,-5,-2));
        model_ground = glm::rotate(model_ground, glm::radians(90.0f), glm::vec3(1,0,0));
        cel::globals::main_shader.set_mat4("model", model_ground);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
    }
};
