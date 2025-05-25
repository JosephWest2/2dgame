#pragma once

#include "abstract/Entity.hpp"

class Player : public Entity {
public:
    Player(Vec2 world_position);
    virtual const string& GetEntityName() override;
};
