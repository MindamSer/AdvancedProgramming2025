#pragma once

#include "math2d.h"

#include <vector>


enum Direction : uint8_t
{
    UP,
    UP_LEFT,
    UP_RIGHT,
    LEFT,
    RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    DOWN,
    COUNT,
    OPP_SUM = COUNT - 1
};

static const int2 directionVectors[] = {
    {0, -1},
    {-1, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {1, 1},
    {0, 1},
};

static const uint32_t directionCosts[] = {
    10,
    14,
    14,
    10,
    10,
    14,
    14,
    10,
};

using Path = std::vector<Direction>;
