#pragma once
#include "EntityRegistry.hpp"
#include "Renderer.hpp"
#include "abstract/Entity.hpp"
#include "structures/Background.hpp"
#include "Camera.hpp"
#include <string>
#include <vector>
#include <memory>
#include <map>

using std::vector;
using std::string;
using std::unique_ptr;
using std::map;

typedef string BackgroundTextureName;
typedef uint ZIndex;

class Level {
public:
    Level(const string& level_name);
    unordered_map<SDL_Texture*, RenderTexture> RenderTextures(Camera& camera) const;

private:
    vector<unique_ptr<Entity>> _entities;
    map<ZIndex, unordered_map<BackgroundTextureName, vector<Background>>> _backgrounds;
};
