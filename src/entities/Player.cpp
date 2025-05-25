#include "Player.hpp"

Player::Player(Vec2 world_position) : Entity{world_position} {}

const string& Player::GetEntityName() {
    static string name = "player";
    return name;
}
