#pragma once

struct Vec2 {
    int x;
    int y;

    Vec2 operator-(const Vec2& other) const {
        return {this->x - other.x, this->y - other.y};
    }
    Vec2 operator+(const Vec2& other) const {
        return {this->x + other.x, this->y + other.y};
    }
};
