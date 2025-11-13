#pragma once

#include "base.h"


class StarvationSystem : BaseSystem
{
public:
    void process(float dt, EntityArchetype &entities) override
    {
        accumulator += dt;
        if (accumulator < damageInterval) return;
        accumulator -= damageInterval;

        for (size_t i = 0; i < entities.size(); ++i)
        {
            auto &health = entities.healthBars[i];
            health.change(-damageAmount);
            if (health.getValue() <= 0)
                entities.deleteAt(i);
        }
    }

private:
    float accumulator = 0.0f;
    const float damageInterval = 1.0f; // seconds
    const int damageAmount = 2; // health points
};