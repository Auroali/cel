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
#include "cel/render/matrix_stack.h"

#include "cel/framework/object.h"
#include "cel/framework/components/component.h"

#include "cel/logger.h"

cel_app* cel_app::inst;
int exitCode;
double cel::time::delta_time = 1;
double cel::time::fixed_delta_time = 1.f/60.f;
std::vector<cel::project_builder_base*>* cel::project::projects;
uint64_t render_engine_flags = CEL_RENDERFLAG_MULTISAMPLE_AA;

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
    case CEL_SIG_CAM_REQ:
        *(cel::camera**)ptr = cel::render::render_engine::get_camera().lock().get();
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
        std::vector<std::weak_ptr<cel::component>> components;
        if(auto scene = cel::scene::get_active_scene().lock()) {
            
            // Add all updateable components to the components vector
            for(auto obj : scene->get_obj_tree().get_sorted()) {
                for(auto c : obj->val->get_components()) {
                    components.push_back(c);
                }
            }
        }
        glfwPollEvents();
        accumulator += cel::time::delta_time;
        for(cel::project* p : projects) {
            p->update();
        }
        // Call update for all updateable components
        for(auto c : components) {
            if(auto c_locked = c.lock()) {
                c_locked->fixed_update();
            }
        }        
        while (accumulator >= cel::time::fixed_delta_time) {
            cel::window::main()->get_input_handler().process_mouse();
		    for(cel::project* p : projects) {
		        p->fixed_update();
                
		    }
            // Step the physics engine
            if(auto scene = cel::scene::get_active_scene().lock())
                if(scene->get_phys())
                    scene->get_phys()->step();
            // Call fixed update for all updateable components
            for(auto c : components) {
                if(auto c_locked = c.lock()) {
                    c_locked->fixed_update();
                }
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
    cel::logger->error(errorPtr);
}

bool cel_app::on_init() {
    if(!glfwInit()) {
        cel::logger->error("Failed to initialize GLFW!");
        print_glfw_err();
        return false;
    }
    
    
    GLFWwindow* handle;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!(handle = glfwCreateWindow(480, 480, "", NULL, NULL))) {
        cel::logger->error("Failed to create window!");
        print_glfw_err();
        return false;
    }

    glfwMakeContextCurrent(handle);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cel::logger->error("Failed to get GLFW proc address!\n");
        return false;
    }

    for(cel::project* p : cel::project::build_projects()) {
        if(!p->init()) {
            cel::logger->error(fmt::format("An error occured whilst initializing projects: Failed to initialize project {}", p->get_name()));
            return false;
        }
        projects.push_back(p);
    }
    
    if(projects.size() <= 0) {
        cel::logger->error("An error occured whilst initializing projects: list was empty");
        return false;
    }
    LOG_INFO(fmt::format("Initialized {} projects!", projects.size()));
    glfwSetWindowTitle(handle, projects[0]->get_name().c_str());
    //this->cam = std::make_shared<cel::camera3d>();

    cel::globals::init_shaders();
    
    win_main = cel::window(handle);
    win_main.set_main();
    
    cel::render::render_engine::init(render_engine_flags);
	return true;
}

void cel_app::render() {
    cel::render::matrix_stack stack;
    cel::render::render_engine::render(stack, projects, cel::scene::get_active_scene());
    
    //Push buffer to screen
    cel::window::main()->swap();
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