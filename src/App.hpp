#pragma once
#include "Renderer.hpp"
#include "GameState.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>

using string = std::string;
using runtime_error = std::runtime_error;

class App {
public:
    App(string window_title)
        : _should_quit{false}
        , _renderer{nullptr} {
        if(!SDL_Init(SDL_INIT_VIDEO)) {
            throw runtime_error("failed to init SDL: " + string(SDL_GetError()));
        }
        _renderer = new Renderer(window_title);
    }
    ~App() {
        delete _renderer;
        SDL_Quit();
    }
    void Run() {
        _renderer->LoadTextures();
        while (!_should_quit) {
            PollEvents();
            _renderer->RenderFrame();
        }
    }
    void PollEvents() {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    _should_quit = true;
                    break;
            }
        }
    }
private:
    bool _should_quit;
    GameState _game_state;
    Renderer* _renderer;
};
