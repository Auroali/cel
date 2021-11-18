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
#define CEL_ERROR_INIT 1
#define CEL_ERROR_SHADERS 2

//Sets ptr to the main camera
#define CEL_CAM_REQ 0x00

class cel_app {
private:
    bool running;
    bool initializedProjects;
    std::vector<cel::project*> projects;
    //GLFWwindow* display;
    std::shared_ptr<cel::camera> cam;
    cel::window win_main;
    static cel_app* inst;
public:
    static void receive_signal(uint64_t sig, void* ptr);
    bool on_init();
    void loop();
    void render();
    void cleanup();
    cel_app();
    int on_execute();
};
namespace cel {
    /**
     * Used to communicate with the engine (e.g. getting the current camera)
     * @param sig The signal ID, valid IDs can be found in the cel.h header
     * @param ptr Additional data to send with the signal (e.g. for a camera request, this would be a cel::camera double pointer)
     **/
    inline void send_signal(uint64_t sig, void* ptr) {
        cel_app::receive_signal(sig, ptr);
    }
}