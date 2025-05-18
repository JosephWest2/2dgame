#pragma once
#include <string>
#include "structures/WorldPosition.hpp"

using string = std::string;

class Entity {
public:
    Entity(const string name, const WorldPosition world_position)
        : _name{name}
        , _world_position{world_position} {}

    const string& GetName() const {
        return _name;
    }
    const WorldPosition& GetWorldPosition() const {
        return _world_position;
    }

private:
    string _name;
    WorldPosition _world_position;
};
