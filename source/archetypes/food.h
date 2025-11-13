#pragma once

#include "components/transform2d.h"
#include "components/sprite.h"
#include "components/food.h"

#include <vector>
#include <algorithm>


struct FoodArchetype
{
    std::vector<Transform2D> positions;
    std::vector<Sprite> sprites;
    std::vector<Food> foodProps;

    void reserve(size_t n)
    {
        positions.reserve(n);
        sprites.reserve(n);
        foodProps.reserve(n);
    }

    void clear()
    {
        positions.clear();
        sprites.clear();
        foodProps.clear();
    }

    size_t size() { return positions.size(); }

    void add(Transform2D position, Sprite sprite, Food food)
    {
        positions.push_back(position);
        sprites.push_back(sprite);
        foodProps.push_back(food);
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
            MOVE_DEL_TO_END(sprites)
            MOVE_DEL_TO_END(foodProps)
        }
        #undef DEL_N_LAST_FROM

        const size_t N = deleteQueue.size();

        #define DEL_N_LAST_FROM(ARR) ARR.erase(ARR.end() - N, ARR.end());
        {
            DEL_N_LAST_FROM(positions)
            DEL_N_LAST_FROM(sprites)
            DEL_N_LAST_FROM(foodProps)
        }
        #undef DEL_N_LAST_FROM

        deleteQueue.clear();
    }

private:
    std::vector<size_t> deleteQueue;
};
