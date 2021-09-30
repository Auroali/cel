#include "window.h"

namespace cel {
    window* window::main_window;
    int window::get_width() {
        int width,height;
        glfwGetWindowSize(handle, &width, &height);
        return width; 
    }
    int window::get_height() { 
        int width,height;
        glfwGetWindowSize(handle, &width, &height);
        return height; 
    }
    void window::set_main() {
        main_window = this;
    }
    void window::swap() {
        glfwSwapBuffers(handle);
    }
    bool window::is_closed() {
        return glfwWindowShouldClose(handle);
    }
    window* window::main() {
        return main_window;
    }
    window::window(GLFWwindow* handle) : handler(handle) {
        this->handle = handle;
    }
    input_handler* window::get_input_handler() {
        return &handler;
    }
    void window::close() {
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }
    void window::set_title(const std::string& title) {
        glfwSetWindowTitle(handle, title.c_str());
    }
    void window::set_cursor_mode(cursor_mode mode)
    {
        switch (mode)
        {
        case cursor_mode::FREE:
            glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        
        case cursor_mode::LOCKED:
            glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }
}