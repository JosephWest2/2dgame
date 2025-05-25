#pragma once

#include "abstract/Entity.hpp"
#include "entities/Player.hpp"
#include <format>
#include <memory>


using std::unique_ptr;
using std::make_unique;
using std::format;

class EntityRegistry {
public:
    static function<unique_ptr<Entity>(Vec2)> GetConstructor(const string& entity_name) {
        if (!_registry.contains(entity_name)) {
            throw runtime_error(format("entity {} is not registered", entity_name));
        }
        return _registry[entity_name];
    }
private:
    inline static unordered_map<string, function<unique_ptr<Entity>(Vec2)>> _registry = {
        {Player::GetEntityName(), [](Vec2 world_position){return make_unique<Player>(world_position);}}
    };
};
