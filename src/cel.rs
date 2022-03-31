mod renderer;
mod input;
mod io;
mod math;
mod framework;

use std::path::Path;
use glfw::Context;
use glm::vec3;
use crate::framework::Camera;

fn main() {
    let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();
    // Set the OpenGL version
    glfw.window_hint(glfw::WindowHint::ContextVersionMajor(4));
    glfw.window_hint(glfw::WindowHint::ContextVersionMinor(5));
    glfw.window_hint(glfw::WindowHint::OpenGlProfile(glfw::OpenGlProfileHint::Core));
    // Create the OpenGL window
    let (mut window, _) = glfw.create_window(800, 800, "Hello", glfw::WindowMode::Windowed).expect("Failed to create window!");
    window.make_current();

    // We can't use window after this call, as it gets moved
    input::init(window);

    // Init renderer
    renderer::init();

    // Print info about the renderer
    #[cfg(debug_assertions)]
    renderer::print_info();
    
    let cam = Camera{ pos: vec3(0.,0.,0.), fov: 45.0 };
    let ship = io::import_obj(Path::new("./test.obj"));
    let shader = io::load_shader(Path::new("./shaders/main.vs"), Path::new("./shaders/main.fs"));

    let mut model = math::mat4(1.);
    model = glm::ext::translate(&model, glm::vec3(0.,-0.5, -4.));
    model = glm::ext::scale(&model, glm::Vector3::new(0.25,0.25,0.25));
    let mut delta;
    let mut last_time = 0.;
    while !input::should_exit() {
        glfw.poll_events();
        delta = glfw.get_time() - last_time;
        last_time = glfw.get_time();

        shader.set_mat4("projection", cam.create_matrix());

        model = glm::ext::rotate(&model, (5. * delta) as f32, vec3(0., 1., 0.));
        shader.use_shader();
        shader.set_mat4("model", model);

        unsafe { gl::ClearColor(0., 0., 0., 0.); gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT); }
        ship.render();
        input::swap_buffers();
    }
}