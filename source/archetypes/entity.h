#pragma once

#include "components/transform2d.h"
#include "components/dungeon_restrictor.h"
#include "components/sprite.h"
#include "components/bar.h"

#include <vector>
#include <algorithm>


struct entityArchetype
{
public:
    std::vector<Transform2D> positions;
    std::vector<DungeonRestrictor> restrictors;
    std::vector<Sprite> sprites;

    std::vector<HealthBar> healthBars;
    std::vector<StaminaBar> staminaBars;

    std::vector<float> accumulatedTimes;
    std::vector<bool> isPredators;

public:
    void reserve(size_t n)
    {
        positions.reserve(n);
        restrictors.reserve(n);
        sprites.reserve(n);

        healthBars.reserve(n);
        staminaBars.reserve(n);

        accumulatedTimes.reserve(n);
        isPredators.reserve(n);

        deleteQueue.reserve(n);
    }

    void addEntity(Transform2D position, DungeonRestrictor dungeon, Sprite sprite, HealthBar health, StaminaBar stamina, bool predator)
    {
        positions.push_back(position);
        restrictors.push_back(dungeon);
        sprites.push_back(sprite);

        healthBars.push_back(health);
        staminaBars.push_back(stamina);

        accumulatedTimes.push_back(0.f);
        isPredators.push_back(predator);
    }

    void queueDeletion(size_t index) { deleteQueue.push_back(index); }
    void performDeletion()
    {
        if (deleteQueue.empty())
            return;

        std::sort(deleteQueue.begin(), deleteQueue.end());

        size_t whereToPlace = positions.size() - 1;
        size_t indexInQueue = deleteQueue.size() - 1;

        #define MOVE_DEL_TO_END(ARR) ARR[deleteQueue[indexInQueue]] = ARR[whereToPlace];
        for (size_t i = 0; i < deleteQueue.size(); ++i, --whereToPlace, --indexInQueue)
        {
            MOVE_DEL_TO_END(positions)
            MOVE_DEL_TO_END(restrictors)
            MOVE_DEL_TO_END(sprites)

            MOVE_DEL_TO_END(healthBars)
            MOVE_DEL_TO_END(staminaBars)

            MOVE_DEL_TO_END(accumulatedTimes)
            MOVE_DEL_TO_END(isPredators)
        }
        #undef DEL_N_LAST_FROM

        const size_t N = positions.size();

        #define DEL_N_LAST_FROM(ARR) ARR.erase(ARR.end() - N, ARR.end());
        {
            DEL_N_LAST_FROM(positions)
            DEL_N_LAST_FROM(restrictors)
            DEL_N_LAST_FROM(sprites)

            DEL_N_LAST_FROM(healthBars)
            DEL_N_LAST_FROM(staminaBars)

            DEL_N_LAST_FROM(accumulatedTimes)
            DEL_N_LAST_FROM(isPredators)
        }
        #undef DEL_N_LAST_FROM

        deleteQueue.clear();
    }

private:
    std::vector<size_t> deleteQueue;
};
