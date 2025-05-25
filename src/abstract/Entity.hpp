#pragma once
#include "structures/Vec2.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

using std::function;
using std::unordered_map;
using std::string;
using std::runtime_error;

class Entity {
public:
    Entity(Vec2 world_position);
    virtual ~Entity() = default;
    virtual const string& GetEntityName() = 0;
    Vec2 world_position;
    static string EntityName;
};

