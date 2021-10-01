#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

namespace cel {
    enum class keyboard : int {
        //Standard Alphabet Keys
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        //Special Keys
        ESC = GLFW_KEY_ESCAPE,
        L_CTRL = GLFW_KEY_LEFT_CONTROL,
        R_CTRL = GLFW_KEY_RIGHT_CONTROL,
        L_SUPER = GLFW_KEY_LEFT_SUPER,
        R_SUPER = GLFW_KEY_RIGHT_SUPER,
        L_ALT = GLFW_KEY_LEFT_ALT,
        R_ALT = GLFW_KEY_RIGHT_ALT,
        L_SHIFT = GLFW_KEY_LEFT_SHIFT,
        R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
        //Arrow Keys
        LEFT_ARROW = GLFW_KEY_LEFT,
        UP_ARROW = GLFW_KEY_UP,
        RIGHT_ARROW = GLFW_KEY_RIGHT,
        DOWN_ARROW = GLFW_KEY_DOWN,
        CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
        //Numerical Keys
        KEY_0 = GLFW_KEY_0,
        KEY_1 = GLFW_KEY_1,
        KEY_2 = GLFW_KEY_2,
        KEY_3 = GLFW_KEY_3,
        KEY_4 = GLFW_KEY_4,
        KEY_5 = GLFW_KEY_5,
        KEY_6 = GLFW_KEY_6,
        KEY_7 = GLFW_KEY_7,
        KEY_8 = GLFW_KEY_8,
        KEY_9 = GLFW_KEY_9,

        KEYPAD_0 = GLFW_KEY_KP_0,
        KEYPAD_1 = GLFW_KEY_KP_1,
        KEYPAD_2 = GLFW_KEY_KP_2,
        KEYPAD_3 = GLFW_KEY_KP_3,
        KEYPAD_4 = GLFW_KEY_KP_4,
        KEYPAD_5 = GLFW_KEY_KP_5,
        KEYPAD_6 = GLFW_KEY_KP_6,
        KEYPAD_7 = GLFW_KEY_KP_7,
        KEYPAD_8 = GLFW_KEY_KP_8,
        KEYPAD_9 = GLFW_KEY_KP_9,

        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,
        F13 = GLFW_KEY_F13,
        F14 = GLFW_KEY_F14,
        F15 = GLFW_KEY_F15,
        F16 = GLFW_KEY_F16,
        F17 = GLFW_KEY_F17,
        F18 = GLFW_KEY_F18,
        F19 = GLFW_KEY_F19,
        F20 = GLFW_KEY_F20,
        F21 = GLFW_KEY_F21,
        F22 = GLFW_KEY_F22,
        F23 = GLFW_KEY_F23,
        F24 = GLFW_KEY_F24,
        F25 = GLFW_KEY_F25,
        //Other
        R_BRACKET = GLFW_KEY_RIGHT_BRACKET,
        L_BRACKET = GLFW_KEY_LEFT_BRACKET,
        COMMA = GLFW_KEY_COMMA,
        SEMICOLON = GLFW_KEY_SEMICOLON,
        TILDE = GLFW_KEY_APOSTROPHE,
        GRAVE = GLFW_KEY_GRAVE_ACCENT,
        PERIOD = GLFW_KEY_PERIOD,
        FORWARD_SLASH = GLFW_KEY_SLASH,
        BACK_SLASH = GLFW_KEY_BACKSLASH,
        ENTER = GLFW_KEY_ENTER,
        MINUS = GLFW_KEY_MINUS,
        EQUALS = GLFW_KEY_EQUAL,
        BACKSPACE = GLFW_KEY_BACKSPACE,
        KEYPAD_ENTER = GLFW_KEY_KP_ENTER,
        KEYPAD_PLUS = GLFW_KEY_KP_ADD,
        KEYPAD_DECIMAL = GLFW_KEY_KP_DECIMAL,
        KEYPAD_DIVISION = GLFW_KEY_KP_DIVIDE,
        KEYPAD_EQUAL = GLFW_KEY_KP_EQUAL,
        KEYPAD_SUBRACT = GLFW_KEY_KP_SUBTRACT,
        KEYPAD_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
        INSERT = GLFW_KEY_INSERT,
        NUM_LOCK = GLFW_KEY_NUM_LOCK,
        MENU = GLFW_KEY_MENU
    };
    enum class mouse : int {
        BUTTON_1 = GLFW_MOUSE_BUTTON_1,
        BUTTON_2 = GLFW_MOUSE_BUTTON_2,
        BUTTON_3 = GLFW_MOUSE_BUTTON_3,
        BUTTON_4 = GLFW_MOUSE_BUTTON_4,
        BUTTON_5 = GLFW_MOUSE_BUTTON_5,
        BUTTON_6 = GLFW_MOUSE_BUTTON_6,
        BUTTON_7 = GLFW_MOUSE_BUTTON_7,
        BUTTON_8 = GLFW_MOUSE_BUTTON_8
    };
    enum class cursor_mode {
        FREE,
        LOCKED
    };
    class input_handler {
        GLFWwindow* handle;
        bool hasMousePos = false;
        double prevMouseX,prevMouseY;
        double mouseXDelta, mouseYDelta;
    public:
        input_handler(GLFWwindow* handle) {
            if(!handle) {
                return;
            } 
            this->handle = handle;
            glfwSetWindowUserPointer(handle, this);
            //glfwSetCursorPosCallback(handle, mouseUpdateCallback);
        }
        void process_mouse() {
            mouseYDelta = 0;
            mouseXDelta = 0;
            
            if(!hasMousePos) {
                hasMousePos = true;
                prevMouseX = get_mouse_x();
                prevMouseY = get_mouse_y();
                return;
            }
            mouseXDelta = get_mouse_x() - prevMouseX;
            mouseYDelta = get_mouse_y() - prevMouseY;
            prevMouseX = get_mouse_x();
            prevMouseY = get_mouse_y();
        }
        double get_mouse_x() {
            double x,y;
            glfwGetCursorPos(handle, &x, &y);
            return x;
        }
        double get_mouse_y() {
            double x,y;
            glfwGetCursorPos(handle, &x, &y);
            return y;
        }
        double get_mouse_delta_x() {
            return mouseXDelta;
        }
        double get_mouse_delta_y() {
            return mouseYDelta;
        }
        bool key_down(keyboard key) {
            return key_down(static_cast<int>(key));
        }
        bool mouse_btn_down(mouse button) {
            return mouse_btn_down(static_cast<int>(button));
        }
        bool key_down(int key) {
            return glfwGetKey(handle, key) == GLFW_PRESS;
        }
        bool mouse_btn_down(int button) {
            return glfwGetMouseButton(handle, button) == GLFW_PRESS;
        }
    };
}