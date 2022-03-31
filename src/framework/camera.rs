use glm::vec3;
use super::super::input;

pub struct Camera {
    pub pos: glm::Vec3,
    pub fov: f32
}

impl Camera {
    pub fn create_matrix(&self) -> glm::Matrix4<f32> {
        let (width, height) = input::get_window_size();
        let proj = glm::ext::perspective::<f32>(glm::radians(self.fov), width as f32 / height as f32, 0.05, 100.);
        let view = glm::ext::look_at::<f32>(self.pos, self.pos + vec3(0.,0.,-1.), vec3(0.,1.,0.));
        return proj.mul_m(&view);
    }
}