#pragma once
#include "cel/render/matrix_stack.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cel/window.h"
#include "cel/globals.h"

namespace cel {
    /**
     * Base class for a camera
     * Has no functionality
     */
    class camera {
    public:
        /**
         * @brief Gets the camera's x position in worldspace
         * @return the x position of the camera
         */
        virtual double get_x() { return 0; }

        /**
         * @brief Gets the camera's y position in worldspace
         * @return the y position of the camera
         */
        virtual double get_y() { return 0; }

        /**
         * @brief Gets the camera's z position in worldspace
         * @return the z position of the camera
         */
        virtual double get_z() { return 0; }

        /**
         * @brief Get the camera's forward vector
         * @return the forward vector
         */
        virtual glm::vec3 get_forward() { return cel::globals::forward; }

        /**
         * @brief Get the camera's up vector
         * @return the up vector
         */
        virtual glm::vec3 get_up() { return cel::globals::up; }

        /**
         * @brief Get the camera's right vector
         * @return the right vector 
         */
        virtual glm::vec3 get_right() { return cel::globals::right; }

        /**
         * @brief Moves the camera to a specified location
         * 
         * @param x the x position to move to
         * @param y the y position to move to
         * @param z the z position to move to
         */
        virtual void set_xyz(double x, double y, double z) {}

        /**
         * @brief Move the camera relative to its position
         * 
         * @param x the distance to move along the x axis
         * @param y the distance to move along the y axis
         * @param z the distance to move along the z axis
         */
        virtual void translate(double x, double y, double z) {}

        /**
         * @brief Sets the camera's position
         * 
         * @param pos the position to move to
         */
        virtual void set_xyz(glm::vec3 pos) {}

        /**
         * @brief Moves the camera relative to its position
         * 
         * @param pos the vector to move the camera by
         */
        virtual void translate(glm::vec3 pos) {}

        /**
         * @brief Sets the euler rotation of the camera
         * @note all values are in radians
         * 
         * @param x the roll of the camera
         * @param y the yaw of the camera
         * @param z the pitch of the camera
         */
        virtual void set_rot_euler(double x, double y, double z) {}

        virtual void shader_setup(cel::render::shader& stack) {}
    };
    
    /**
     * Camera for viewing 2D space
     */
    class camera2d : public camera {
        glm::vec2 pos;
    public:
        /**
         * @brief Gets the camera's x position in worldspace
         * @return the x position of the camera
         */
        virtual double get_x() { return pos.x; }

        /**
         * @brief Gets the camera's y position in worldspace
         * @return the y position of the camera
         */
        virtual double get_y() { return pos.y; }

        /**
         * @brief Gets the camera's z position in worldspace
         * @return the z position of the camera
         */
        virtual double get_z() { return -15; }

        /**
         * @brief Get the camera's forward vector
         * @return the forward vector
         */
        virtual glm::vec3 get_forward() { return cel::globals::forward; }

        /**
         * @brief Get the camera's up vector
         * @return the up vector
         */
        virtual glm::vec3 get_up() { return cel::globals::up; }
        
        /**
         * @brief Get the camera's right vector
         * @return the right vector 
         */
        virtual glm::vec3 get_right() { return cel::globals::right; }
        
        /**
         * @brief Moves the camera to a specified location
         * 
         * @note The z value will be ignored
         * 
         * @param x the x position to move to
         * @param y the y position to move to
         * @param z the z position to move to
         */
        virtual void set_xyz(double x, double y, double z) {
            pos = glm::vec2(x,y);
        }

        /**
         * @brief Move the camera relative to its position
         * 
         * @note The z value will be ignored
         * 
         * @param x the distance to move along the x axis
         * @param y the distance to move along the y axis
         * @param z the distance to move along the z axis
         */
        virtual void translate(double x, double y, double z) {
            pos += glm::vec2(x,y);
        }

        /**
         * Sets the camera's position
         * 
         * @note The z value will be ignored
         * 
         * @param pos the position to move to
         */
        virtual void set_xyz(glm::vec3 pos) {
            this->pos = glm::vec2(pos.x, pos.y);
        }

        /**
         * Moves the camera relative to its position
         * 
         * @note The z value will be ignored
         * 
         * @param pos the vector to move the camera by
         */
        virtual void translate(glm::vec3 pos) {
            this->pos += glm::vec2(pos.x, pos.y);
        }

        /**
         * @note This isn't implemented for the 2D camera, as it cannot rotate
         */
        virtual void set_rot_euler(double x, double y, double z) {}

        virtual void shader_setup(cel::render::shader& stack) {}
    };

    /**
     * Camera for viewing 3D space
     */
    class camera3d : public camera {
    private:
        double x,y,z;
        glm::mat4 proj_view_matrix;
        glm::vec3 forward = cel::globals::forward,right = cel::globals::right;
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
            forward = glm::quat(glm::vec3(x,y,z)) * cel::globals::forward;
            right = glm::normalize(glm::cross(forward, cel::globals::up));
        }
        /**
         * Setup shader specific things, such as projection/view matrix
         * @param shader pointer to the shader to use
         */
        virtual void shader_setup(cel::render::shader& stack) override {
            glm::vec3 pos = glm::vec3((float)x, (float)y, (float)z);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)cel::window::main()->get_width() / (float)cel::window::main()->get_height(), 0.05f, 100.f);
            glm::mat4 view = glm::lookAt(
                pos,
                pos + forward, 
                cel::globals::up
            );
            // Done using shader instead of matrix_stack so that we can still get the worldpos of whatever is being rendered in the shader
            stack.set_mat4("projection_view", proj * view);
        }
    };
}