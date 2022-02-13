#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "project.h"
#include "framework/camera.h"
#include "memory"
#include "window.h"

//Error codes
#define CEL_ERROR_INIT          0x01
#define CEL_ERROR_SHADERS       0x02
#define CEL_ERROR_FRAMEBUFFER   0x03
#define CEL_ERROR_FILEIO        0x04

class cel_app {
private:
    bool running;
    bool initializedProjects;
    std::vector<cel::project*> projects;
    //GLFWwindow* display;
    cel::window win_main;
    static cel_app* inst;
public:
    bool on_init();
    void render();
    void cleanup();
    cel_app();
    int on_execute();
};

/**
 * @namespace cel Contains all cel code
 */
namespace cel {
    /**
     * @brief Exits with code `code`
     * 
     * @param code exit code to use
     */
    void request_exit(int code);
}

