#pragma once
#include <string>
#include <memory>
#include <map>
#include "cel/io/stb_image.h"
#include <filesystem>
#include <optional>

#include <GLFW/glfw3.h>

namespace cel::render {
    class texture {
    private:
        unsigned int tex;
        unsigned int w,h;
    public:
        texture() {}

        texture(unsigned char* data, int x, int y, int channels) {
            if(data) {
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D, tex);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                w = x;
                h = y;

                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        texture(int width, int height) {
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            
            w = width;
            h = height;
        }
        texture(unsigned int id, int width, int height) {
            tex = id;
            w = width;
            h = height;
        }
        void resize(int width, int height) {
            h = height;
            w = width;
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        bool is_valid() {
            return glIsTexture(tex);
        }
        unsigned int handle() {
            return tex;
        }
        void free() {
            glDeleteTextures(1, &tex);
        }
        static std::optional<texture> load(const std::filesystem::path& name) {
            if(std::filesystem::exists(name) && std::filesystem::is_regular_file(name)) {
                int x,y,channels;
                unsigned char* data = stbi_load(name.c_str(), &x, &y, &channels, 0);
                texture tex = texture(data, x, y, channels);
                stbi_image_free(data);
                if(tex.is_valid()) {
                    return tex;
                }
            }
            return {};
        }
        int width() {
            return w;
        }
        int height() {
            return h;
        }
        operator int() {
            return this->tex;
        }
    };
}