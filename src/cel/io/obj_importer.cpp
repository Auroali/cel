#include "obj_importer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<std::string> split(std::string str, char delimeter=' '){
    std::string t = std::string();
    std::vector<std::string> str_v;
    for(char c : str) {
        if(c != delimeter) {
            t += c;
        } else if(t != std::string() + delimeter && t != std::string()) {
            str_v.push_back(t);
            t = "";
        }
    }
    str_v.push_back(t);

	if(str_v.size() == 0) {
		str_v.push_back(std::string() + delimeter);
	}

    return str_v;
}

namespace cel::io {
    std::optional<cel::render::model> import_obj_model(const std::filesystem::path& file) {
        if(std::filesystem::exists(file) && std::filesystem::is_regular_file(file)) {
            std::ifstream stream(file);
            std::string line;

            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> normals;

            while(std::getline(stream, line)) {
                std::stringstream ss(line);
                std::istream_iterator<std::string> begin(ss);
                std::istream_iterator<std::string> end;
                std::vector<std::string> vstrings(begin, end);
                if(vstrings.size() <= 0)
                    continue;
                if(vstrings[0] == "v")
                    vertices.push_back(glm::vec3(std::stof(vstrings[1]),std::stof(vstrings[2]),std::stof(vstrings[3])));
                if(vstrings[0] == "vt")
                    uvs.push_back(glm::vec2(std::stof(vstrings[1]),std::stof(vstrings[2])));
                if(vstrings[0] == "vn")
                    normals.push_back(glm::vec3(std::stof(vstrings[1]),std::stof(vstrings[2]),std::stof(vstrings[3])));
            }
            stream.clear();
            stream.seekg(0, std::ios::beg);
            std::vector<render::vertex> model_data;
            while(std::getline(stream, line)) {
                std::stringstream ss(line);
                std::istream_iterator<std::string> begin(ss);
                std::istream_iterator<std::string> end;
                std::vector<std::string> vstrings(begin, end);

                if(vstrings.size() <= 0)
                    continue;
                if(vstrings[0] == "f") {
                    render::vertex v1;
                    render::vertex v2;
                    render::vertex v3;
                    // Vertex 1
                    std::vector<std::string> face = split(vstrings[1], '/');
                    if(!face[0].empty())
                        v1.pos = vertices[std::stoull(face[0])-1];
                    if(face.size() >= 2 && !face[1].empty())
                        v1.uv = uvs[std::stoull(face[1])-1];
                    if(face.size() >= 3 && !face[2].empty())
                        v1.normals = normals[std::stoull(face[2])-1];
                    // Vertex 2
                    face = split(vstrings[2], '/');
                    if(!face[0].empty())
                        v2.pos = vertices[std::stoull(face[0])-1];
                    if(face.size() >= 2 && !face[1].empty())
                        v2.uv = uvs[std::stoull(face[1])-1];
                    if(face.size() >= 3 && !face[2].empty())
                        v2.normals = normals[std::stoull(face[2])-1];
                    // Vertex 3
                    face = split(vstrings[3], '/');
                    if(!face[0].empty())
                        v3.pos = vertices[std::stoull(face[0])-1];
                    if(face.size() >= 2 && !face[1].empty())
                        v3.uv = uvs[std::stoull(face[1])-1];
                    if(face.size() >= 3 && !face[2].empty())
                        v3.normals = normals[std::stoull(face[2])-1];
                    model_data.push_back(v1);
                    model_data.push_back(v2);
                    model_data.push_back(v3);
                }
            }
            stream.close();
            vertices.clear();
            uvs.clear();
            normals.clear();
            unsigned int vao = 0;
            unsigned int vbo = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, model_data.size() * sizeof(render::vertex), model_data.data(), GL_STATIC_DRAW);

            // Pos
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::vertex), (void*)0);
            glEnableVertexAttribArray(0);
            // UVs
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(render::vertex), (void*)offsetof(render::vertex, uv));
            glEnableVertexAttribArray(1);
            // Normals
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(render::vertex), (void*)offsetof(render::vertex, normals));
            glEnableVertexAttribArray(2);
            

            glBindVertexArray(0);
            return cel::render::model(vao,model_data.size());
        }
        return {};
    }
}