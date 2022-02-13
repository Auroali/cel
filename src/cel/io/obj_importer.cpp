#include "obj_importer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iterator>
#include "cel/logger.h"
#include "cel/util.h"

namespace cel::io {
    std::optional<cel::render::model> import_obj_model(const std::filesystem::path& file) {
        if(!std::filesystem::exists(file)) {
            cel::logger->error(fmt::format("File at '{}' doesn't exist", file.string()));
            // An error occured, return nothing
            return {};
        }
        std::ifstream stream(file);
        std::string line;

        // Arrays for the verticex positions, uvs, and normals
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
        // Reset to the start of the stream, then iterate line-by-line and contruct all the faces
        stream.clear();
        stream.seekg(0, std::ios::beg);
        std::vector<render::vertex> model_data;
     // Note that cel assumes all faces on the model are triangulated
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
                std::vector<std::string> face = cel::split(vstrings[1], '/');
                if(!face[0].empty())
                    v1.pos = vertices[std::stoull(face[0])-1];
                if(face.size() >= 2 && !face[1].empty())
                    v1.uv = uvs[std::stoull(face[1])-1];
                if(face.size() >= 3 && !face[2].empty())
                    v1.normals = normals[std::stoull(face[2])-1];
                // Vertex 2
                face = cel::split(vstrings[2], '/');
                if(!face[0].empty())
                    v2.pos = vertices[std::stoull(face[0])-1];
                if(face.size() >= 2 && !face[1].empty())
                    v2.uv = uvs[std::stoull(face[1])-1];
                if(face.size() >= 3 && !face[2].empty())
                    v2.normals = normals[std::stoull(face[2])-1];
                // Vertex 3
                face = cel::split(vstrings[3], '/');
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
        // Close the stream, then clear all arrays
        stream.close();
        vertices.clear();
        uvs.clear();
        normals.clear();
        // Build the VAO and VBO
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
        
        // Unbind the vertex array
        glBindVertexArray(0);
        // Return the model
        return cel::render::model(vao, model_data.size());
       }
}