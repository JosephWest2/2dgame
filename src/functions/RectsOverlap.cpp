#include "RectsOverlap.hpp"

bool RectsOverlap(SDL_FRect& first, SDL_FRect& second) {
    float first_upper_x = first.x + first.w;
    float second_upper_x = second.x + second.w;
    float first_upper_y = first.y + first.h;
    float second_upper_y = second.y + second.h;
    if (first_upper_x < second.x
            || first.x < second_upper_x
            || first.y > second_upper_y
            || first_upper_y < second.y) {
        return false;
    }  
    return true;
}
