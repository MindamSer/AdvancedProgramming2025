#pragma once

#include "math2d.h"


struct Transform2D
{
public:
    Transform2D(float x=0, float y=0, float sizeX=1, float sizeY=1)
        : x(x), y(y), sizeX(sizeX), sizeY(sizeY) {}

    Transform2D(int2 xy)
        : Transform2D(xy.x, xy.y) {}

public:
    float x, y;
    float sizeX, sizeY;
};
