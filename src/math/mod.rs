pub fn mat4(val: f32) -> glm::Matrix4<f32> {
    return glm::Mat4::new(
        glm::Vector4::new(val, 0., 0., 0.),
        glm::Vector4::new(0., val, 0., 0.),
        glm::Vector4::new(0., 0., val, 0.),
        glm::Vector4::new(0., 0., 0., val));
}