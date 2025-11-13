#pragma once

#include "archetypes/entity.h"
#include "archetypes/food.h"
#include <variant>


class FoodSystem
{
public:
    void process(float dt, EntityArchetype &entities, FoodArchetype &foods)
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (entities.isPredators[i])
                continue;

            auto &eaterPos = entities.positions[i];

            for (size_t j = 0; j < foods.size(); ++j)
            {
                auto &foodPos = foods.positions[j];
                if (eaterPos.x == foodPos.x &&
                    eaterPos.y == foodPos.y)
                {
                    auto foodProp = foods.foodProps[j];

                    if (std::holds_alternative<HeathFood>(foodProp))
                        entities.healthBars[i].change(std::get<HeathFood>(foodProp).value);

                    if (std::holds_alternative<StaminaFood>(foodProp))
                        entities.staminaBars[i].change(std::get<StaminaFood>(foodProp).value);

                    foods.deleteAt(j);
                }
            }
        }
    }
};
