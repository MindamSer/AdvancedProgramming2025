#pragma once

#include "components/transform2d.h"
#include "components/sprite.h"

#include <vector>


struct tileArchetype
{
    std::vector<Transform2D> positions;
    std::vector<Sprite> sprites;

    void reserve(size_t n)
    {
        positions.reserve(n);
        sprites.reserve(n);
    }

    void add(Transform2D position, Sprite sprite)
    {
        positions.push_back(position);
        sprites.push_back(sprite);
    }

    void clear()
    {
        positions.clear();
        sprites.clear();
    }
};