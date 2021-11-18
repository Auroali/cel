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
// Current cel_app instance
cel_app* cel_app::inst;

int exitCode;

unsigned int quad;
cel::render::framebuffer render_buffer;

double cel::time::deltaTime = 1;
double cel::time::fixedDeltaTime = 1.f/60.f;

class exit_request : std::exception {};

cel::project* cel::project_builder_base::build() {
    return new project();
}
std::vector<cel::project_builder_base*>* cel::project::projects;

void cel::request_exit(int code) {
    exitCode = code;
    throw exit_request();
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
            loop();
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
        if(!p->init())
            std::cerr << "Failed to initialize project " << p->get_name() << "\n";
        projects.push_back(p);
    }
    
    if(projects.size() <= 0) {
        std::cerr << "An error occured whilst initializing projects: list was empty\n";
        return false;
    }
    std::cout << "Initialized " << projects.size() << " projects!" << std::endl;
    glfwSetWindowTitle(handle, projects[0]->get_name().c_str());
    this->cam = std::make_shared<cel::camera3d>();

    cel::constants::init_shaders();

    
    win_main = cel::window(handle);
    win_main.set_main();
    
    render_buffer = cel::render::framebuffer(win_main.get_height(), win_main.get_width());

    float vertices[] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };	
    unsigned int vao;
    unsigned int vbo = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    

    glBindVertexArray(0);
    quad = vao;

    return true;
}
glm::vec3 pos;
void cel_app::loop() {
    cel::window::main()->get_input_handler()->process_mouse();
    for(cel::project* p : projects) {
        p->fixed_update();
    }
}

void cel_app::render() {
    glViewport(0,0,cel::window::main()->get_width(),cel::window::main()->get_height());
    if(render_buffer.get_texture().width() != cel::window::main()->get_width() || render_buffer.get_texture().height() != cel::window::main()->get_height()) {
        std::cout << "Render Buffer size doesn't match screen size! Resizing... (Expected " << cel::window::main()->get_width() << ',' << cel::window::main()->get_height() << " but got " << render_buffer.tex.width() << ',' << render_buffer.tex.height() << ")" << std::endl;
        render_buffer = cel::render::framebuffer(cel::window::main()->get_width(),cel::window::main()->get_height());
    }

    render_buffer.bind();
    cel::constants::main_shader.use();
    cam->render_start();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f,0,0,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //Set things up for the shader (view/projection matrix, etc)
    cam->shader_setup(&cel::constants::main_shader);
    //Call render() for all initialized projects
    for(cel::project* p : projects) {
        p->render();
    }
    render_buffer.unbind();

    // Render quad
    cel::constants::quad_shader.use();
    cel::constants::quad_shader.set_tex("sceneTex", 0);
    glClearColor(0,1.0f,0,1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_DEPTH_TEST);
    
    //quad.render();
    glBindVertexArray(quad);
    glBindTexture(GL_TEXTURE_2D, render_buffer.get_texture());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

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