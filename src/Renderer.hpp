#pragma once
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

using string = std::string;
using runtime_error = std::runtime_error;
template <typename A, typename B>
using unordered_map = std::unordered_map<A, B>;

class Renderer {
public:
    Renderer(string window_title)
        : _window{nullptr}
        , _renderer{nullptr}
        , _surface{nullptr} {
        _window = SDL_CreateWindow(window_title.c_str(), 640, 480, 0);
        if (_window == nullptr) {
            throw runtime_error("failed to create window: " + string(SDL_GetError()));
        }
        _renderer = SDL_CreateRenderer(_window, nullptr);
        if (_renderer == nullptr) {
            throw runtime_error("failed to create renderer: " + string(SDL_GetError()));
        }
    }
    ~Renderer() {
        CleanupTextures();
        if (_renderer != nullptr) {
            SDL_DestroyRenderer(_renderer);
        }
        if (_window != nullptr) {
            SDL_DestroyWindow(_window);
        }
    }
    SDL_Texture* LoadTexture(const char* file_path) {
        SDL_Texture* texture = nullptr;
        texture = IMG_LoadTexture(_renderer, file_path);
        return texture;
    }
    void LoadTextures() {
        _textures["player"] = LoadTexture("res/Cute_Fantasy_Free/Player/Player.png");
        if (_textures["player"] == nullptr) {
            throw runtime_error("failed to load player texture");
        }
    }
    void CleanupTextures() {
        for (auto& [_, texture] : _textures) {
            SDL_DestroyTexture(texture);
        }
        _textures.clear();
    }
    void RenderFrame() {
        SDL_RenderClear(_renderer);

        SDL_FRect player_sprite_rect{
            .x = 0.f,
            .y = 0.f,
            .w = 32.f,
            .h = 32.f
        };
        SDL_FRect player_screen_space_location{
            .x = 50.f,
            .y = 50.f,
            .w = 100.f,
            .h = 100.f
        };
        SDL_RenderTexture(_renderer, _textures["player"], &player_sprite_rect, &player_screen_space_location);
        SDL_FRect r{10.f, 10.f, 50.f, 50.f};
        SDL_RenderRect(_renderer, &r);
        SDL_RenderPresent(_renderer);
    }
private:
    unordered_map<string, SDL_Texture*> _textures;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Surface* _surface;
};
