#pragma once
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <unordered_map>

using string = std::string;
using runtime_error = std::runtime_error;
template <typename A, typename B>
using unordered_map = std::unordered_map<A, B>;

class App {
public:
    App(string window_title)
        : _should_quit{false}
        , _window{nullptr}
        , _renderer{nullptr}
        , _surface{nullptr}
        , _textures{}
    {
        if(!SDL_Init(SDL_INIT_VIDEO)) {
            throw runtime_error("failed to init SDL: " + string(SDL_GetError()));
        }
        _window = SDL_CreateWindow(window_title.c_str(), 640, 480, 0);
        if (_window == nullptr) {
            throw runtime_error("failed to create window: " + string(SDL_GetError()));
        }
        _renderer = SDL_CreateRenderer(_window, nullptr);
        if (_renderer == nullptr) {
            throw runtime_error("failed to create renderer: " + string(SDL_GetError()));
        }
    }
    ~App() {
        if (_renderer != nullptr) {
            SDL_DestroyRenderer(_renderer);
        }
        if (_window != nullptr) {
            SDL_DestroyWindow(_window);
        }
        SDL_Quit();
    }
    void Run() {
        LoadTextures();
        while (!_should_quit) {
            RenderFrame();
            PollInputs();
        }
    }
    void PollInputs() {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    _should_quit = true;
                    break;
            }
        }
    }
    SDL_Texture* LoadTexture(const char* file_path) {
        SDL_Texture* texture = nullptr;
        texture = IMG_LoadTexture(_renderer, file_path);
        return texture;
    }
    void LoadTextures() {
        _textures["player"] = LoadTexture("res/Cute_Fantasy_Free/Tiles/Grass_Middle.png");
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
        if (!SDL_RenderTexture(_renderer, _textures["player"], nullptr, nullptr)) {
            throw runtime_error("failed to render texture: " + string(SDL_GetError()));
        }
        SDL_FRect r{10.f, 10.f, 50.f, 50.f};
        SDL_RenderRect(_renderer, &r);
        SDL_RenderPresent(_renderer);
    }
private:
    unordered_map<string, SDL_Texture*> _textures;
    bool _should_quit;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Surface* _surface;
};
