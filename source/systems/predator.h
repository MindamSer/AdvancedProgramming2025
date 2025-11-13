#pragma once

#include "base.h"


class PredatorSystem : BaseSystem
{
public:
    void process(float dt, EntityArchetype &entities) override
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (!entities.isPredators[i])
                continue;

            auto &predPos = entities.positions[i];

            for (size_t j = 0; j < entities.size(); ++j)
            {
                if (i == j || entities.isPredators[j])
                    continue;

                auto &preyPos = entities.positions[j];
                if (predPos.x == preyPos.x &&
                    predPos.y == preyPos.y)
                {
                    auto &predHealth = entities.healthBars[i];
                    auto &preyHealth = entities.healthBars[j];

                    predHealth.change(preyHealth.getValue());
                    entities.deleteAt(j);
                }
            }
        }
    }
};