#pragma once

#include "game_object.h"
#include "math2d.h"

class IRestrictor : public Component {
public:
    virtual bool can_pass(int2 coordinates) = 0;
};
