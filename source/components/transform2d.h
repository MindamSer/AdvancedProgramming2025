#pragma once


struct Transform2D
{
public:
    Transform2D(float x=0, float y=0, float sizeX=1, float sizeY=1)
        : x(x), y(y), sizeX(sizeX), sizeY(sizeY) {}

public:
    float x, y;
    float sizeX, sizeY;
};
