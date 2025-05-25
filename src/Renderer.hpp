#pragma once
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "Level.hpp"
#include "structures/RenderTexture.hpp"

using std::string;
using std::runtime_error;
using std::unordered_map;
using std::vector;

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
        if (_renderer != nullptr) {
            SDL_DestroyRenderer(_renderer);
        }
        if (_window != nullptr) {
            SDL_DestroyWindow(_window);
        }
    }
    void RenderFrame(vector<RenderTexture>& textures) {
        SDL_RenderClear(_renderer);
        for (const auto& texture : textures) {
            SDL_RenderTexture(_renderer, texture.texture, &texture.dst, &texture.dst);
        }
        SDL_RenderPresent(_renderer);
    }
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Surface* _surface;
};
