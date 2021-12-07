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
#if ENABLE_RENDERER
#include "cel/render/model.h"
#include "cel/render/framebuffer.h"
#include "cel/render/texture.h"
#include "cel/render/renderer.h"
#include "cel/render/matrix_stack.h"
#endif

cel_app* cel_app::inst;
int exitCode;
double cel::time::delta_time = 1;
double cel::time::fixed_delta_time = 1.f/60.f;
std::vector<cel::project_builder_base*>* cel::project::projects;
uint64_t render_engine_flags = 0;

#if ENABLE_RENDERER
/**
 * Definitions for variables and functions in constants.h
 */
cel::render::shader cel::globals::main_shader;
cel::render::shader cel::globals::quad_shader;
cel::render::shader cel::globals::basic_shader;
#endif

void cel::globals::init_shaders() {
    #if ENABLE_RENDERER
    main_shader = cel::render::shader("./assets/main.vs", "./assets/main.fs");
    quad_shader = cel::render::shader("./assets/post.vs", "./assets/post.fs");
    basic_shader = cel::render::shader("./assets/basic.vs", "./assets/basic.fs");
    #endif
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
    case CEL_SIG_CAM_REQ:
        #if ENABLE_RENDERER
        *(cel::camera**)ptr = cel::render::render_engine::get_camera().lock().get();
        #endif
        break;
    case CEL_SIG_RENDER_PARAMS:
        render_engine_flags = *reinterpret_cast<uint64_t*>(ptr);
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
    cel::time::delta_time = 0.01;
	double current = glfwGetTime();
	double accumulator = 0.0;
    while(!cel::window::main()->is_closed()) {
        
        double new_time = glfwGetTime();
        cel::time::delta_time = new_time - current;
        current = new_time;
                
        glfwPollEvents();
        accumulator += cel::time::delta_time;
        for(cel::project* p : projects) {
            p->update();
        }
        while (accumulator >= cel::time::fixed_delta_time) {
            cel::window::main()->get_input_handler().process_mouse();
		    for(cel::project* p : projects) {
		        p->fixed_update();
		    }
            accumulator -= cel::time::fixed_delta_time;
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
    #if ENABLE_RENDERER
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
    #endif

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
    #if ENABLE_RENDERER
    glfwSetWindowTitle(handle, projects[0]->get_name().c_str());
    //this->cam = std::make_shared<cel::camera3d>();

    cel::globals::init_shaders();
    
    win_main = cel::window(handle);
    win_main.set_main();
    
    cel::render::render_engine::init(render_engine_flags);
    #endif
	return true;
}

void cel_app::render() {
    #if ENABLE_RENDERER
    cel::render::matrix_stack stack;
    cel::render::render_engine::render(stack, projects, cel::scene::get_active_scene());
    
    //Push buffer to screen
    cel::window::main()->swap();
    #endif
}

void cel_app::cleanup() {
    for(cel::project* p : projects) {
        p->shutdown();
        delete p;
    }
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

#if ENABLE_RENDERER
void cel::render::render_engine::set_flags(uint64_t flags) {
    render_engine_flags = flags;
}
#endif