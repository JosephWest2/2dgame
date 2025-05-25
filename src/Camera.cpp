#include "Camera.hpp"

Camera::Camera(int view_width, int view_height)
    : CameraPosition{0,0}
    , ViewWidth{view_width}
    , ViewHeight{view_height} {}

Vec2 Camera::WorldToCameraSpace(Vec2 world_position) {
    return world_position - CameraPosition + Vec2{ViewWidth / 2, ViewHeight / 2};
}
