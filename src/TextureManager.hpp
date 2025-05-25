#pragma once
#include "components/EntityComponent.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>
#include <format>

#define TEXTURE_FILEPATH = "data/textures.json"

using std::string;
using std::runtime_error;
using std::unordered_map;
using std::ifstream;
using nlohmann::json;
using std::format;

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

    void LoadTexture(EntityComponent& entity, SDL_Renderer* renderer) {
        auto entity_name = entity.GetName();
        auto entity_filepath = _entity_filepaths[entity_name];
        auto texture_filename = _texture_root_dir + entity_filepath;
        if (_textures.contains(entity_name)) {
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
        constexpr const char* texture_filepath = "data/textures.json";
        ifstream texture_file{texture_filepath};
        if (texture_file.fail()) {
            throw runtime_error(format("failed to load file {}", texture_filepath));
        }
        auto texture_data = json::parse(texture_file);
        if (!texture_data.contains("entities") || !texture_data["entities"].is_object()) {
            throw runtime_error(format("{} does not contain an 'entities' object", texture_filepath));
        }
        if (!texture_data["root"] || !texture_data["root"].is_string()) {
            throw runtime_error(format("{} does not contain a 'root' string", texture_filepath));
        }
        _texture_root_dir = texture_data["root"];
        for (auto& [entity_name, filepath] : texture_data["entities"].items()) {
            _entity_filepaths[entity_name] = filepath;
        }
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
    unordered_map<EntityName, FilePath> _entity_filepaths;
};
