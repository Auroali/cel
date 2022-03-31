mod shader;

pub use shader::Shader;

use gl::types::*;
use std::os::raw::c_char;
use std::mem;

pub struct Vertex {
    pub pos: glm::Vec3,
    pub normal: glm::Vec3,
    pub uv: glm::Vec2
}

pub struct Model {
    pub id: u32,
    pub size: usize
}

impl Model {
    pub fn render(&self) {
        draw_vao(self.id, self.size);
    }
}

pub fn gen_vao(geom: Vec<Vertex>) -> Model {
    let mut arr: GLuint = 0;
    let arr_ptr: *mut GLuint = &mut arr;
    unsafe {
        gl::GenVertexArrays(1, arr_ptr);
        gl::BindVertexArray(arr);

        let mut buf: GLuint = 0;
        let buf_ptr: *mut GLuint = &mut buf;
        gl::GenBuffers(1, buf_ptr);
        gl::BindBuffer(gl::ARRAY_BUFFER, buf);
        gl::BufferData(gl::ARRAY_BUFFER, geom.len().checked_mul(mem::size_of::<Vertex>()).expect("Overflow while getting size!") as isize, geom.as_ptr() as *const GLvoid, gl::STATIC_DRAW);

        gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE, mem::size_of::<Vertex>() as GLint, 0 as *const GLvoid);
        gl::EnableVertexAttribArray(0);
        gl::VertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE, mem::size_of::<Vertex>() as GLint, 0 as *const GLvoid);
        gl::EnableVertexAttribArray(1);
        gl::VertexAttribPointer(2, 3, gl::FLOAT, gl::FALSE, mem::size_of::<Vertex>() as GLint, 0 as *const GLvoid);
        gl::EnableVertexAttribArray(2);
        gl::BindVertexArray(0);
    }
    return Model { id: arr, size: geom.len() * 3 };

}

fn draw_vao(vao: u32, size: usize) {
    unsafe {
        gl::BindVertexArray(vao);
        gl::DrawArrays(gl::TRIANGLES, 0, size as i32);
        gl::BindVertexArray(0);
    }
}

pub fn init() {
    gl_loader::init_gl();
    gl::load_with(|symbol| gl_loader::get_proc_address(symbol) as *const _);
    unsafe {
        gl::Enable(gl::DEPTH_TEST);
    }
}

pub fn print_info() {
    println!("GLFW: {}", glfw::get_version_string());
    unsafe {
        println!("OpenGL: {}", glfw::string_from_c_str((gl::GetString(gl::VERSION)) as *const c_char));
    }
}