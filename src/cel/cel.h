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

//Sets ptr to the main camera
#define CEL_SIG_CAM_REQ        0x00
#define CEL_SIG_RENDER_PARAMS  0x01

class cel_app {
private:
    bool running;
    bool initializedProjects;
    std::vector<cel::project*> projects;
    //GLFWwindow* display;
    cel::window win_main;
    static cel_app* inst;
public:
    static void receive_signal(uint64_t sig, void* ptr);
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
     * @brief Used to communicate with the engine
     * <br>
     * <br> Available signals are:
     * <br> CEL_SIG_CAM_REQ: Takes a `cel::camera**` in `ptr`, will set to the current active camera
     * <br> CEL_SIG_RENDER_PARAMS: Takes an `uint64_t*` in `ptr`, will set the render flags to this value. Only works in `cel::project::init`
     * @param sig The signal ID, valid IDs can be found in the cel.h header
     * @param ptr Additional data to send with the signal (e.g. for a camera request, this would be a cel::camera double pointer)
     * @deprecated
     **/
    [[deprecated("Use cel::render::render_engine::get_camera() or cel::render::render_engine::set_flags() instead")]]
    inline void send_signal(uint64_t sig, void* ptr) {
        cel_app::receive_signal(sig, ptr);
    }
    /**
     * @brief Exits with code `code`
     * 
     * @param code exit code to use
     */
    void request_exit(int code);
}

/** @mainpage Cel
 *  @section info Info
 *  Cel is an open source game engine, developed primarily for linux.
 *  @section setup Setup
 *  Linux:                                                                                                                  <br>
 *      Clone the repository with `git clone https://github.com/Jacgar8/cel.git`                                            <br>
 *      `cd` into the directory and run `make` to build cel                                                                 <br>
 *      All output files can be found in the `build` directory.
 *  @section use Using Cel
 *  To use cel, create a class derived from `cel::project` and set up `cel::project_builder`                            <br>
 *  The project is now ready. To compile it, you must link with libcel.so (Linux), which can be found in the `build` directory. <br>
 *  An example project is included with cel, in `src/celtest`
 * 
 *  @section name Naming
 *  Function names are written in snake case <br>
 *  E.g. `void do_stuff(int param)`          <br>
 *                                           <br>
 *  Variable names are written in snake case <br>
 *  E.g. `int variable_name`                  <br>
 *                                           <br> 
 *  Functions/Classes beginning with two underscores are intended for internal use, but exposed in the event that they are needed. It is not recommended to use these.
 * 
 */