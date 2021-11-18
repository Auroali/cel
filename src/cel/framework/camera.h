#pragma once
#include "cel/render/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cel/window.h"
#include "cel/constants.h"

namespace cel {
    class camera {
    public:
        virtual double get_x() { return 0; }
        virtual double get_y() { return 0; }
        virtual double get_z() { return 0; }

        virtual glm::vec3 get_forward() { return cel::constants::forward; }
        virtual glm::vec3 get_up() { return cel::constants::up; }
        virtual glm::vec3 get_right() { return cel::constants::right; }

        virtual void set_xyz(double x, double y, double z) {}
        virtual void translate(double x, double y, double z) {}
        virtual void set_xyz(glm::vec3 pos) {}
        virtual void translate(glm::vec3 pos) {}
        virtual void set_rot_euler(double x, double y, double z) {}
        virtual void render_start() {}
        virtual void shader_setup(cel::render::shader* shader) {}
    };

    class camera3d : public camera {
    private:
        double x,y,z;
        glm::mat4 proj_view_matrix;
        glm::vec3 forward = cel::constants::forward,right = cel::constants::right;
    public:
        virtual double get_x() override { return x; }
        virtual double get_y() override { return y; }
        virtual double get_z() override { return z; }

        virtual void set_xyz(double x, double y, double z) override {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        virtual void translate(double x, double y, double z) override {
            this->x += x;
            this->y += y;
            this->z += z;
        }
        virtual void set_xyz(glm::vec3 pos) override {
            this->x = pos.x;
            this->y = pos.y;
            this->z = pos.z;
        }
        virtual void translate(glm::vec3 pos) override {
            this->x += pos.x;
            this->y += pos.y;
            this->z += pos.z;
        }
        virtual glm::vec3 get_forward() override { 
            return forward; 
        }
        virtual glm::vec3 get_up() override { 
            return glm::normalize(glm::cross(forward, right)); 
        }
        virtual glm::vec3 get_right() override { 
            return right;
        }
        virtual void set_rot_euler(double x, double y, double z) override {
            forward = glm::quat(glm::vec3(x,y,z)) * cel::constants::forward;
            right = glm::normalize(glm::cross(forward, cel::constants::up));
        }
        virtual void render_start() override {
            glm::vec3 pos = glm::vec3((float)x, (float)y, (float)z);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)cel::window::main()->get_width() / (float)cel::window::main()->get_height(), 0.05f, 100.f);
            glm::mat4 view = glm::lookAt(
                pos,
                pos + forward, 
                cel::constants::up
            );
            proj_view_matrix = proj * view;
        }
        virtual void shader_setup(cel::render::shader* shader) override {
            shader->set_mat4("projView", proj_view_matrix);
        }
    };
}