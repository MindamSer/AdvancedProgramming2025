#pragma once

#include "game_object.h"
#include "transform2d.h"
#include <SDL3/SDL_rect.h>

struct Camera2D : Component {
    Transform2D position;
    float scale;
    Camera2D(float x=0.f, float y=0.f, float pixelsPerMeter=1.f)
        : position(x,y), scale(pixelsPerMeter) {}

    SDL_FRect to_camera_space(const Transform2D& object_transform) const
    {
        return SDL_FRect{
            (object_transform.x - position.x) * scale,
            (object_transform.y - position.y) * scale,
            object_transform.sizeX * scale,
            object_transform.sizeY * scale,
        };
    }
};