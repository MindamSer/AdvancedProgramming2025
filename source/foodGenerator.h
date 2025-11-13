#pragma once

#include "archetypes/food.h"
#include "components/food.h"
#include "dungeon.h"


class FoodGenerator
{
public:
    struct WeightedFood
    {
        Food food;
        Sprite sprite;
        float weight = 1.f;

        WeightedFood(Food _food, Sprite _sprite, float _weight = 1.f)
            : food(_food), sprite(_sprite), weight(_weight) {}
    };

public:
    FoodGenerator(Dungeon *_dungeon, FoodArchetype *_foods, const std::vector<WeightedFood> &weightedVariants, float _cooldown, size_t initialAmount = 0);

    void update(float dt);

private:
    Dungeon *dungeon;
    FoodArchetype *foods;
    std::vector<WeightedFood> variants;
    const float cooldown;
    float elapsedTime = 0.f;
};
