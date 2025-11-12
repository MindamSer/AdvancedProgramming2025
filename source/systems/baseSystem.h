#pragma once

#include "archetypes/entity.h"


class BaseSystem
{
public:
    virtual void process(float dt, EntityArchetype &entities) = 0;
};