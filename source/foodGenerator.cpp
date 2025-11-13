#include "foodGenerator.h"


FoodGenerator::FoodGenerator(Dungeon *_dungeon, FoodArchetype *_foods, const std::vector<WeightedFood> &weightedVariants, float _cooldown, size_t initialAmount)
    : dungeon(_dungeon), foods(_foods), variants(weightedVariants), cooldown(_cooldown)
{
    float sum = 0.f;
    for (const WeightedFood wf : variants)
        sum += wf.weight;
    for (auto &p : variants)
        p.weight /= sum;

    for(size_t k = 0; k < initialAmount; ++k)
    {
        size_t i = 0;
        for(float p = float(rand() % 100 + 1) / 100.f; p > 0.f; ++i)
            p -= variants[i].weight;
        --i;

        foods->add(
            dungeon->getRandomFloorPosition(),
            variants[i].sprite,
            variants[i].food
        );
    }
}

void FoodGenerator::update(float dt)
{
    elapsedTime += dt;
    if (elapsedTime < cooldown)
        return;
    elapsedTime -= cooldown;

    size_t i = 0;
    for(float p = float(rand() % 100 + 1) / 100.f; p > 0.f; ++i)
        p -= variants[i].weight;
    --i;

    foods->add(
        dungeon->getRandomFloorPosition(),
        variants[i].sprite,
        variants[i].food
    );
}