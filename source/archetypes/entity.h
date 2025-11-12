#pragma once

#include "components/transform2d.h"
#include "components/sprite.h"
#include "components/dungeonPtr.h"
#include "components/bar.h"
#include "components/camera2d.h"

#include <vector>
#include <algorithm>


struct EntityArchetype
{
public:
    std::vector<Transform2D> positions;
    std::vector<Sprite> sprites;
    std::vector<Dungeon *> dungeonPtrs;

    std::vector<HealthBar> healthBars;
    std::vector<StaminaBar> staminaBars;

    std::vector<bool> isBots;
    std::vector<bool> isPredators;
    std::vector<Camera2D *> cameras;

    std::vector<float> moveCooldowns;

public:
    void reserve(size_t n)
    {
        positions.reserve(n);
        sprites.reserve(n);
        dungeonPtrs.reserve(n);

        healthBars.reserve(n);
        staminaBars.reserve(n);

        isBots.reserve(n);
        isPredators.reserve(n);
        cameras.reserve(n);

        moveCooldowns.reserve(n);

        deleteQueue.reserve(n);
    }

    void clear()
    {
        positions.clear();
        sprites.clear();
        dungeonPtrs.clear();

        healthBars.clear();
        staminaBars.clear();

        isBots.clear();
        isPredators.clear();
        cameras.clear();

        moveCooldowns.clear();

        deleteQueue.clear();
    }

    size_t size() { return positions.size(); }

    size_t add(Transform2D position, Sprite sprite, Dungeon *dungeonPtr = nullptr, HealthBar health = {100}, StaminaBar stamina = {100}, bool bot = true, bool predator = false, Camera2D *cameraPtr = nullptr)
    {
        positions.push_back(position);
        dungeonPtrs.push_back(dungeonPtr);
        sprites.push_back(sprite);

        healthBars.push_back(health);
        staminaBars.push_back(stamina);

        isBots.push_back(bot);
        isPredators.push_back(predator);
        cameras.push_back(cameraPtr);

        moveCooldowns.push_back(0.f);

        return positions.size();
    }


    void deleteAt(size_t index) { deleteQueue.push_back(index); }

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
            MOVE_DEL_TO_END(dungeonPtrs)
            MOVE_DEL_TO_END(sprites)

            MOVE_DEL_TO_END(healthBars)
            MOVE_DEL_TO_END(staminaBars)

            MOVE_DEL_TO_END(moveCooldowns)
            MOVE_DEL_TO_END(isPredators)
        }
        #undef DEL_N_LAST_FROM

        const size_t N = positions.size();

        #define DEL_N_LAST_FROM(ARR) ARR.erase(ARR.end() - N, ARR.end());
        {
            DEL_N_LAST_FROM(positions)
            DEL_N_LAST_FROM(dungeonPtrs)
            DEL_N_LAST_FROM(sprites)

            DEL_N_LAST_FROM(healthBars)
            DEL_N_LAST_FROM(staminaBars)

            DEL_N_LAST_FROM(moveCooldowns)
            DEL_N_LAST_FROM(isPredators)
        }
        #undef DEL_N_LAST_FROM

        deleteQueue.clear();
    }

private:
    std::vector<size_t> deleteQueue;
};
