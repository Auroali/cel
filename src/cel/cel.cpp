#include <iostream>
#include "project.h"
#include <vector>
#include "cel.h"
#include "time.h"
#include "framework/camera.h"
#include "input/input.h"
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "io/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#include "cel/render/model.h"
#include "cel/render/framebuffer.h"
#include "cel/render/texture.h"
#include "cel/render/renderer.h"

cel_app* cel_app::inst;
int exitCode;
double cel::time::deltaTime = 1;
double cel::time::fixedDeltaTime = 1.f/60.f;
std::vector<cel::project_builder_base*>* cel::project::projects;

/**
 * Definitions for variables and functions in constants.h
 */
cel::render::shader cel::globals::main_shader;
cel::render::shader cel::globals::quad_shader;
cel::render::shader cel::globals::basic_shader;

void cel::globals::init_shaders() {
    main_shader = cel::render::shader("./assets/main.vs", "./assets/main.fs");
    quad_shader = cel::render::shader("./assets/post.vs", "./assets/post.fs");
    basic_shader = cel::render::shader("./assets/basic.vs", "./assets/basic.fs");
}

class exit_request : std::exception {};

cel::project* cel::project_builder_base::build() {
    return new project();
}

void cel::request_exit(int code) {
    exitCode = code;
    throw exit_request();
}

void cel_app::receive_signal(uint64_t sig, void* ptr) {
    switch (sig)
    {
    case CEL_CAM_REQ:
        *(cel::camera**)ptr = inst->cam.get();
        break;
    default:
        break;
    }
}

double cel::time::get_current() {
    return glfwGetTime();
}

cel_app::cel_app() : win_main(nullptr) {
    running = true;
    inst = this;
}

int cel_app::on_execute() {
    if(!on_init()) {
        cleanup();
        return CEL_ERROR_INIT;
    }
    cel::time::deltaTime = 0.01;
	double current = glfwGetTime();
	double accumulator = 0.0;
    while(!cel::window::main()->is_closed()) {
        
        double new_time = glfwGetTime();
        cel::time::deltaTime = new_time - current;
        current = new_time;
                
        glfwPollEvents();
        accumulator += cel::time::deltaTime;
        while (accumulator >= cel::time::fixedDeltaTime) {
            cel::window::main()->get_input_handler()->process_mouse();
		    for(cel::project* p : projects) {
		        p->fixed_update();
		    }
            accumulator -= cel::time::fixedDeltaTime;
        }
        render();
    }
    cleanup();
    return exitCode;
}

void print_glfw_err() {
    const char* errorPtr = NULL;
    glfwGetError(&errorPtr);
    std::cerr << errorPtr << '\n';
}

bool cel_app::on_init() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        print_glfw_err();
        return false;
    }
    
    
    GLFWwindow* handle;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(handle = glfwCreateWindow(480, 480, "", NULL, NULL))) {
        std::cerr << "Failed to create window!\n";
        print_glfw_err();
        return false;
    }

    glfwMakeContextCurrent(handle);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to get GLFW proc address!\n";
        return false;
    }

    for(cel::project* p : cel::project::build_projects()) {
        if(!p->init()) {
            std::cerr << "An error occured whilst initializing projects: Failed to initialize project " << p->get_name() << "\n";
            return false;
        }
        projects.push_back(p);
    }
    
    if(projects.size() <= 0) {
        std::cerr << "An error occured whilst initializing projects: list was empty\n";
        return false;
    }
    std::cout << "Initialized " << projects.size() << " projects!" << std::endl;
    glfwSetWindowTitle(handle, projects[0]->get_name().c_str());
    this->cam = std::make_shared<cel::camera3d>();

    cel::globals::init_shaders();
    
    win_main = cel::window(handle);
    win_main.set_main();

    cel::render::render_engine::init();

	return true;
}

void cel_app::render() {
    cel::render::render_engine::render(cam.get(), projects, cel::scene::get_active_scene());
    
    //Push buffer to screen
    cel::window::main()->swap();
}

void cel_app::cleanup() {
    for(cel::project* p : projects)
        delete p;
    glfwTerminate();
}

int main() {
    try {
        cel_app app;
        return app.on_execute();
    }
    catch (exit_request const&) {
        return exitCode;
    }
}
