extern crate glfw;

use std::ffi::{CStr, CString};
use super::renderer::Model;
use super::renderer::Vertex;
use super::renderer::Shader;

use std::fs::File;
use std::io::{BufReader, SeekFrom};
use std::io::prelude::*;
use std::os::raw::c_char;
use std::path::Path;
use gl::types::{GLchar, GLsizei};

use glm::{Vec2, vec2, vec3, Vec3};
use crate::renderer::gen_vao;

pub fn import_obj(path: &Path) -> Model {
    // Vectors to store position, normal, and uv info
    let mut pos: Vec<Vec3> = Vec::new();
    let mut norms: Vec<Vec3> = Vec::new();
    let mut uvs: Vec<Vec2> = Vec::new();
    let mut verts: Vec<Vertex> = Vec::new();

    // Open the obj file for reading
    let file: File = match File::open(path) { Err(why) => panic!("Couldn't open file: {}", why), Ok(file) => file };
    let mut reader = BufReader::new(file);

    // Read all positions, normals and uvs
    for l in reader.by_ref().lines() {
        let line = l.expect("An error occurred whilst parsing file {}");
        let mut split = line.split_ascii_whitespace();
        let id = split.next().unwrap();
        if id == "v" {
            pos.push(vec3(
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string"),
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string"),
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string")
            ));
        }
        else if id == "vn" {
            norms.push(vec3(
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string"),
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string"),
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string")
            ));
        }
        else if id == "vt" {
            uvs.push(vec2(
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string"),
                split.next().unwrap().parse::<f32>().expect("An error occurred whilst parsing string")
            ));
        }
    }
    // Seek back to the start
    match reader.seek(SeekFrom::Start(0)) { Err(why) => panic!("An error occurred: {}", why), _ => {}}

    // Read all faces
    for l in reader.by_ref().lines() {
        let line = l.expect("An error occurred whilst parsing file {}");
        let mut split = line.split_ascii_whitespace();
        if split.next().unwrap() == "f" {
            for _ in 0..3 {
                let mut indices = split.next().unwrap().split("/");
                let mut ind = indices.next().unwrap();
                let mut _pos: Vec3 = vec3(0.,0.,0.);
                let mut _normals: Vec3 = vec3(0.,0.,0.);
                let mut _uvs: Vec2 = vec2(0.,0.);
                // Pos
                if ind.len() > 0 {
                    _pos = pos[ind.parse::<usize>().expect("An error occurred whilst parsing string") - 1];
                }
                // UVs
                ind = indices.next().unwrap();
                if ind.len() > 0 {
                    _uvs = uvs[ind.parse::<usize>().expect("An error occurred whilst parsing string") - 1];
                }
                // Normals
                ind = indices.next().unwrap();
                if ind.len() > 0 {
                    _normals = norms[ind.parse::<usize>().expect("An error occurred whilst parsing string") - 1];
                }
                verts.push(Vertex{ pos: _pos, normal: _normals, uv: _uvs });
            }
        }
    }
    return gen_vao(verts);
}

pub fn load_shader(vertex: &Path, fragment: &Path) -> Shader {
    // Open shader files
    let mut vfile: File = match File::open(vertex) { Err(why) => panic!("Couldn't open file: {}", why), Ok(file) => file };
    let mut ffile: File = match File::open(fragment) { Err(why) => panic!("Couldn't open file: {}", why), Ok(file) => file };

    // Read shaders into string
    let mut vstr = String::new();
    let mut fstr = String::new();
    vfile.read_to_string(&mut vstr).expect("Couldn't read file to string!");
    ffile.read_to_string(&mut fstr).expect("Couldn't read file to string!");

    // Create the shaders
    unsafe {
        let buf = [0i8; 512];
        let mut success = 0;
        let vid = gl::CreateShader(gl::VERTEX_SHADER);
        let vstr_c = CString::new(vstr).expect("An error occurred whilst converting to C-style string!");
        gl::ShaderSource(vid, 1, &vstr_c.as_ptr(), std::ptr::null());
        gl::CompileShader(vid);
        gl::GetShaderiv(vid, gl::COMPILE_STATUS, &mut success);
        if success == 0 {
            gl::GetShaderInfoLog(vid, 512, 0 as *mut GLsizei, buf.as_ptr() as *mut GLchar);
            panic!("{}", CStr::from_ptr(&buf as *const c_char).to_str().expect("Shader compilation error!"));

        }
        let fid = gl::CreateShader(gl::FRAGMENT_SHADER);
        let fstr_c = CString::new(fstr).expect("An error occurred whilst converting to C-style string!");
        gl::ShaderSource(fid, 1, &fstr_c.as_ptr(), std::ptr::null());
        gl::CompileShader(fid);
        gl::GetShaderiv(fid, gl::COMPILE_STATUS, &mut success);
        // Is it bad? Yes. Do I care? Yes.
        if success == 0 {
            gl::GetShaderInfoLog(fid, 512, 0 as *mut GLsizei, buf.as_ptr() as *mut GLchar);
            panic!("{}", CStr::from_ptr(&buf as *const c_char).to_str().expect("Shader compilation error!"));
        }
        let program = gl::CreateProgram();
        gl::AttachShader(program, vid);
        gl::AttachShader(program, fid);
        gl::LinkProgram(program);
        gl::GetProgramiv(vid, gl::LINK_STATUS, &mut success);
        if success == 0 {
            gl::GetProgramInfoLog(vid, 512, 0 as *mut GLsizei, buf.as_ptr() as *mut GLchar);
            panic!("{}", CStr::from_ptr(&buf as *const c_char).to_str().expect("Shader compilation error!"));

        }
        gl::DeleteShader(vid);
        gl::DeleteShader(fid);
        return Shader{ id: program };
    }
}

