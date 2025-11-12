#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "image.h"


struct Sprite
{
public:
    Sprite()
        : texture(nullptr), src{0, 0, 0, 0} {}
    Sprite(TexturePtr texture, const SDL_FRect& src)
        : texture(texture), src(src) {}

public:
    TexturePtr texture;
    SDL_FRect src;
};

inline void draw_strite(SDL_Renderer* renderer, const Sprite& sprite, const SDL_FRect &dst)
{
    if (sprite.texture)
        SDL_RenderTexture(renderer, sprite.texture.get(), &sprite.src, &dst);
}
