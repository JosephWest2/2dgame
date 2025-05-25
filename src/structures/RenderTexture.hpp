#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

struct RenderTexture {
    SDL_Texture* texture;
    SDL_FRect src;
    SDL_FRect dst;
};
