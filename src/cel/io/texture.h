#pragma once
#include <string>
#include <memory>
#include <map>
#include "stb_image.h"
#include <filesystem>

#include <GLFW/glfw3.h>

namespace cel {
    class texture {
    private:
        unsigned int tex;
        static std::map<std::string, std::shared_ptr<texture>> textures;
    public:
        texture(unsigned char* data, int x, int y, int channels) {
            if(data) {
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D, tex);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                

                glBindTexture(GL_TEXTURE_2D, 0);
            }
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
        static std::weak_ptr<texture> get_or_load(const std::string& name) {
            std::filesystem::path img = std::filesystem::weakly_canonical(std::filesystem::path(name));
            std::map<std::string, std::shared_ptr<texture>>::iterator it = textures.find(img.string());
            if(it != textures.end())
                return it->second;
            else {
                if(std::filesystem::exists(img) && std::filesystem::is_regular_file(img)) {
                    int x,y,channels;
                    unsigned char* data = stbi_load(img.c_str(), &x, &y, &channels, 0);
                    std::shared_ptr<texture> tex = std::make_shared<texture>(data, x, y, channels);
                    stbi_image_free(data);
                    if(tex->is_valid()) {
                        textures[img.string()] = tex;
                        return tex;
                    }
                }
            }
            return std::shared_ptr<texture>();
        }
    };
}