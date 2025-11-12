#pragma once

#include "archetypes/entity.h"

#include "dungeon.h"
#include "math2d.h"


const int2 directions[4] = {
    {1,0},
    {-1,0},
    {0,1},
    {0,-1}
};


class MoveSystem
{
public:
    void process(float dt, EntityArchetype &entities)
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            auto &moveCooldown = entities.moveCooldowns[i];
            auto stamina = entities.staminaBars[i];

            moveCooldown += dt * stamina.getSpeed();
            if (moveCooldown < 1.0f)
                continue;
            moveCooldown -= 1.0f;

            auto &position = entities.positions[i];
            auto dungeonPtr = entities.dungeonPtrs[i];
            auto isBot = entities.isBots[i];

            if (isBot)
            {
                int2 move = directions[rand() % 4];

                if (dungeonPtr && dungeonPtr->canPass(int2(static_cast<int>(position.x) + move.x, static_cast<int>(position.y) + move.y)))
                {
                    position.x += move.x;
                    position.y += move.y;
                }
            }
            else
            {
                const bool* keys = SDL_GetKeyboardState(nullptr);
                auto camera = entities.cameras[i];

                int2 move;
                bool moved = false;
                if (keys[SDL_SCANCODE_W]) { move.y -= 1; moved = true; }
                if (keys[SDL_SCANCODE_S]) { move.y += 1; moved = true; }
                if (keys[SDL_SCANCODE_A]) { move.x -= 1; moved = true; }
                if (keys[SDL_SCANCODE_D]) { move.x += 1; moved = true; }
                if (!moved)
                    continue;

                if (dungeonPtr && dungeonPtr->canPass(int2(static_cast<int>(position.x) + move.x, static_cast<int>(position.y) + move.y)))
                {
                    position.x += move.x;
                    position.y += move.y;
                }

                if (camera)
                    camera->position = position;
            }
        }
    }
};