use std::ffi::CString;

pub struct Shader {
    pub id: u32
}

impl Shader {
    pub fn use_shader(&self) {
        unsafe {
            gl::UseProgram(self.id);
        }
    }
    pub fn set_mat4(&self, name: &str, matrix: glm::Mat4) {
        unsafe {
            let name = CString::new(name).expect("An error occurred whilst convert to a C-style string");
            gl::UniformMatrix4fv(gl::GetUniformLocation(self.id, name.as_ptr()), 1, gl::FALSE, &matrix[0][0]);
        }
    }
}