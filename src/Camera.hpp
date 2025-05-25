#include "structures/Vec2.hpp"
#include <sys/types.h>

class Camera {
public:
    Vec2 CameraPosition;
    int ViewWidth;
    int ViewHeight;

    Camera(int view_width = 20, int view_height = 20);

    Vec2 WorldToCameraSpace(Vec2 world_position);
};
