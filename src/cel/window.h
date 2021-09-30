#pragma once
#include <GLFW/glfw3.h>
#include "input/input.h"
#include <string>

namespace cel {
    class window {
    private:
        GLFWwindow* handle;
        input_handler handler;
        static window* main_window;
    public:
        int get_width();
        int get_height();

        void swap();
        bool is_closed();
        void close();

        void set_title(const std::string& title);
        void set_cursor_mode(cursor_mode mode);
        input_handler* get_input_handler();
        void set_main();
        static window* main();
        window(GLFWwindow* handle);
    };
}