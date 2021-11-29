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
        /**
         * @brief Get the width of the window
         * 
         * @return int the width of the window
         */
        int get_width();
        /**
         * @brief Get the height of the window
         * 
         * @return int the height of the window
         */
        int get_height();

        /**
         * @brief Swaps the window's buffers
         * 
         */
        void swap();
        /**
         * @brief Checks whether or not the window is closed
         * 
         * @return true if the window is closed
         * @return false if the window is still open
         */
        bool is_closed();
        /**
         * @brief Closes the window
         * 
         * @note If this is the main window, this will also exit the application
         */
        void close();

        /**
         * @brief Sets the window's title
         * 
         * @param title the title to set the window to
         */
        void set_title(const std::string& title);
        /**
         * @brief Sets the window's cursor mode
         * 
         * @param mode the cursor mode to set
         */
        void set_cursor_mode(cursor_mode mode);
        /**
         * @brief Gets the window's input handler
         * 
         * @return input_handler& a pointer to the window's input handler
         */
        input_handler& get_input_handler();
        /**
         * @brief Sets this as the main window
         * 
         */
        void set_main();
        /**
         * @brief Gets the main window
         * 
         * @return window* a pointer to the main window
         */
        static window* main();
        /**
         * @brief Construct a new window object
         * 
         * @param handle the handle for the GLFW window
         */
        window(GLFWwindow* handle);
    };
}