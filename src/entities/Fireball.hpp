#include "components/EntityComponent.hpp"
#include "entities/Player.hpp"

class Fireball : public Entity {
    Fireball(const Vec2& position)
        : _entity{"fireball", position} {
    }

private:
    EntityComponent _entity;
};
