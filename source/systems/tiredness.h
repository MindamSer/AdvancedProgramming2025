#pragma once

#include "base.h"


class TirednessSystem : BaseSystem
{
public:
    void process(float dt, EntityArchetype &entities) override
    {
        accumulator += dt;
        if (accumulator < damageInterval) return;
        accumulator -= damageInterval;

        for (size_t i = 0; i < entities.size(); ++i)
        {
            auto &stamina = entities.staminaBars[i];
            stamina.change(-damageAmount);
        }
    }

private:
    float accumulator = 0.0f;
    const float damageInterval = 1.0f; // seconds
    const int damageAmount = 5; // health points
};