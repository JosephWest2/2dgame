#pragma once
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"
#include <SDL3/SDL.h>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

constexpr int PLAYER_SPRITE_SIZE = 32;
constexpr int SKELETON_SPRITE_SIZE = 32;
constexpr int FIREBALL_SPRITE_SIZE = 128;
constexpr SDL_FRect DEFAULT_PLAYER_SPRITE = {0, 0, PLAYER_SPRITE_SIZE, PLAYER_SPRITE_SIZE};
constexpr SDL_FRect DEFAULT_SKELETON_SPRITE = {0, 0, SKELETON_SPRITE_SIZE, SKELETON_SPRITE_SIZE};

#define PLAYER_KEY "player"
#define GRASS_KEY "grass"
#define FIREBALL_KEY "fireball"
#define OAK_TREE_KEY "oak_tree"
#define DIRT_KEY "dirt"
#define SKELETON_KEY "skeleton"

#define PLAYER_TEXTURE "res/textures/player/player.png"
#define GRASS_TEXTURE "res/textures/outdoor/grass.png"
#define DIRT_TEXTURE "res/textures/outdoor/dirt.png"
#define OAK_TREE_TEXTURE "res/textures/outdoor/oak_tree.png"
#define SKELETON_TEXTURE "res/textures/enemies/skeleton.png"

ostream& operator<<(ostream& os, const SDL_FRect& frect) {
    os << frect.x << ',' << frect.y << ',' << frect.w << ',' << frect.h << endl;
    return os;
}

struct TextureInstance {
    SDL_FRect src{};
    SDL_FRect dst{};
    float rotation{};
    SDL_FlipMode flip_mode{SDL_FLIP_NONE};
};

struct Vec2 {
    int x{};
    int y{};

    Vec2 operator-(const Vec2& other) const { return {this->x - other.x, this->y - other.y}; }
    Vec2 operator+(const Vec2& other) const { return {this->x + other.x, this->y + other.y}; }
};

class Camera {
  public:
    int view_width{};
    int view_height{};

    Camera(int view_width = 20, int view_height = 20)
        : view_height{view_height}, view_width{view_width} {}

    Vec2 WorldToCameraSpace(Vec2 world_position, Vec2 camera_position) {
        return world_position - camera_position + Vec2{view_width / 2, view_height / 2};
    }
};

class IEntity;

vector<weak_ptr<IEntity>>* EntityRegistry() {
    static auto instance = new vector<weak_ptr<IEntity>>{};
    return instance;
}

struct EntityComponent {
    Vec2 position{};
    int texture_width{};
    int texture_height{};
    SDL_FRect sprite_src{};
    SDL_FlipMode flip_mode{SDL_FLIP_NONE};
    float rotation{};

    SDL_FRect SpriteDst() {
        float x = position.x - (float)texture_width / 2;
        float y = position.y - (float)texture_height / 2;
        return {x, y, (float)texture_width, (float)texture_height};
    }
};

class IEntity : public enable_shared_from_this<IEntity> {
  public:
    IEntity() { EntityRegistry()->push_back(shared_from_this()); }
    virtual EntityComponent& GetEntityComponent() = 0;
    virtual const string& GetEntityKey() = 0;
};

class ICollision;

vector<weak_ptr<ICollision>>* CollisionRegistry() {
    static auto instance = new vector<weak_ptr<ICollision>>{};
    return instance;
}

struct CollisionComponent {
    SDL_FRect collision_bounds{};
};

class ICollision : public enable_shared_from_this<ICollision> {
  public:
    ICollision() { CollisionRegistry()->push_back(shared_from_this()); }
    virtual CollisionComponent& GetCollisionComponent() = 0;
};

class Player : public IEntity, public ICollision {
  public:
    Player(Vec2 position) : _entity_component{position, 100, 100, DEFAULT_PLAYER_SPRITE} {};
    virtual EntityComponent& GetEntityComponent() override { return _entity_component; }
    virtual const string& GetEntityKey() override {
        static string name = PLAYER_KEY;
        return name;
    }
    virtual CollisionComponent& GetCollisionComponent() override { return _collision_component; }

  private:
    EntityComponent _entity_component{{100, 100}, 100, 100, DEFAULT_PLAYER_SPRITE};
    CollisionComponent _collision_component{100, 100};
};

class Skeleton : public IEntity, public ICollision {
  public:
    Skeleton(Vec2 position) : _entity_component{position, 100, 100, DEFAULT_SKELETON_SPRITE} {};
    virtual EntityComponent& GetEntityComponent() override { return _entity_component; };
    virtual const string& GetEntityKey() override {
        static string name = SKELETON_KEY;
        return name;
    };
    virtual CollisionComponent& GetCollisionComponent() override { return _collision_component; };

  private:
    EntityComponent _entity_component{.position = {200, 200},
                                      .texture_width = 100,
                                      .texture_height = 100,
                                      .sprite_src = DEFAULT_SKELETON_SPRITE};
    CollisionComponent _collision_component{100, 100};
};

class App {
  public:
    App(string window_title) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw runtime_error("failed to init SDL: " + string(SDL_GetError()));
        }
        _window = SDL_CreateWindow(window_title.c_str(), 640, 480, 0);
        if (!_window) {
            throw runtime_error("failed to create window: " + string(SDL_GetError()));
        }
        _renderer = SDL_CreateRenderer(_window, nullptr);
        if (!_renderer) {
            throw runtime_error("failed to create renderer: " + string(SDL_GetError()));
        }
        LoadTextures();
        LoadEnemies();
    }
    void LoadEnemies() {
        for (int i = 0; i < 5; i++) {
            _skeletons.push_back(make_shared<Skeleton>(Vec2{i * 50,i * 50}));
        };
    }
    ~App() {
        for (const auto& [_, texture] : _textures) {
            SDL_DestroyTexture(texture);
        }
        if (_renderer) {
            SDL_DestroyRenderer(_renderer);
        }
        if (_window) {
            SDL_DestroyWindow(_window);
        }
        SDL_Quit();
    }
    void LoadTextures() {
        _textures[PLAYER_KEY] = LoadTextureFile(PLAYER_TEXTURE);
        _textures[GRASS_KEY] = LoadTextureFile(GRASS_TEXTURE);
        _textures[DIRT_KEY] = LoadTextureFile(DIRT_TEXTURE);
        _textures[OAK_TREE_KEY] = LoadTextureFile(OAK_TREE_TEXTURE);
        _textures[SKELETON_KEY] = LoadTextureFile(SKELETON_TEXTURE);
    }
    SDL_Texture* LoadTextureFile(string file_path) {
        auto texture = IMG_LoadTexture(_renderer, file_path.c_str());
        if (!texture) {
            throw runtime_error("failed to load texture: " + file_path);
        }
        return texture;
    }
    void Run() {
        while (!_should_quit) {
            _delta_time = chrono::high_resolution_clock::now() - _last_frame_time;
            _last_frame_time = chrono::high_resolution_clock::now();
            PollEvents();
            auto instances = PrepareTextureInstances();
            RenderFrame(instances);
        }
    }
    unordered_map<SDL_Texture*, vector<TextureInstance>> PrepareTextureInstances() {
        unordered_map<SDL_Texture*, vector<TextureInstance>> output{};
        for (const auto& entity_wp : *EntityRegistry()) {
            if (auto entity_sp = entity_wp.lock()) {
                EntityComponent& ec = entity_sp->GetEntityComponent();
                output[_textures[entity_sp->GetEntityKey()]].push_back(
                    {ec.sprite_src, ec.SpriteDst(), ec.rotation, ec.flip_mode});
            }
        }
        return output;
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
    void RenderFrame(unordered_map<SDL_Texture*, vector<TextureInstance>> texture_instances) {
        SDL_RenderClear(_renderer);
        for (const auto& [texture, instances] : texture_instances) {
            for (const auto& instance : instances) {
                SDL_RenderTextureRotated(_renderer, texture, &instance.src, &instance.dst,
                                         instance.rotation, nullptr, instance.flip_mode);
            }
        }
        SDL_RenderPresent(_renderer);
    }

  private:
    bool _should_quit{false};
    SDL_Window* _window{};
    SDL_Renderer* _renderer{};
    unordered_map<string, SDL_Texture*> _textures{};
    Camera _camera{};
    chrono::high_resolution_clock::time_point _last_frame_time{
        chrono::high_resolution_clock::now()};
    chrono::duration<double> _delta_time{};
    shared_ptr<Player> _player{make_shared<Player>(Vec2{100, 100})};
    vector<shared_ptr<Skeleton>> _skeletons{};
};
