#pragma once

#include <SDL3/SDL_rect.h>

#include "transform2d.h"


struct Camera2D
{
public:
    Camera2D(float x=0.f, float y=0.f, float pixelsPerMeter=1.f)
        : position(x,y), scale(pixelsPerMeter) {}

    SDL_FRect toCameraSpace(const Transform2D& object_transform) const
    {
        return SDL_FRect {
            (object_transform.x - position.x) * scale,
            (object_transform.y - position.y) * scale,
            object_transform.sizeX * scale,
            object_transform.sizeY * scale,
        };
    }

public:
    Transform2D position;
    float scale;
};
