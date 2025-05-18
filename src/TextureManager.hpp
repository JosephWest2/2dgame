#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <toml.hpp>

using string = std::string;
using runtime_error = std::runtime_error;
template <typename A, typename B>
using unordered_map = std::unordered_map<A, B>;
using ifstream = std::ifstream;

typedef string FilePath;
typedef string EntityName;

class TextureManager {
public:
    static TextureManager& Get() {
        static TextureManager instance{};
        return instance;
    }
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    void LoadTexture(string texture_filename, SDL_Renderer* renderer) {
        if (_textures.contains(texture_filename)) {
            return;
        }
        auto texture = IMG_LoadTexture(renderer, texture_filename.c_str());
        if (texture == nullptr) {
            throw runtime_error("failed to load texture: " + texture_filename);
        }
        _textures[texture_filename] = texture;
    }
    void UnloadTexture(string texture_filename) {
        auto texture = _textures[texture_filename];
        if (texture == nullptr) {
            return;
        }
        SDL_DestroyTexture(texture);
        _textures.erase(texture_filename);
    }
private:
    TextureManager() {
        auto texture_data = toml::parse("data/textures.toml");
        
    }
    ~TextureManager() {
        for (auto& [_, texture] : _textures) {
            if (texture != nullptr) {
                SDL_DestroyTexture(texture);
            }
        }
    }

private:
    unordered_map<EntityName, SDL_Texture*> _textures;
    FilePath _texture_root_dir;
    unordered_map<EntityName, FilePath> _entity_file_paths;
};
