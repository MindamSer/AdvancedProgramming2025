#pragma once

#include "base.h"

#include "components/control.h"

#include "dungeon.h"
#include "math2d.h"


class MoveSystem : BaseSystem
{
public:
    void process(float dt, EntityArchetype &entities) override
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            auto &moveCooldown = entities.moveCooldowns[i];
            auto stamina = entities.staminaBars[i];

            moveCooldown += dt * stamina.getSpeed();
            if (moveCooldown < 1.0f)
                continue;
            moveCooldown -= 1.0f;

            const auto input = entities.controls[i];

            if (input == NONE)
                continue;

            auto &position = entities.positions[i];
            auto dungeonPtr = entities.dungeonPtrs[i];

            switch (input)
            {
                case KEYBOARD:
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
                break;

                case AI:
                {
                    auto &path = entities.paths[i];

                    if(!path.empty())
                    {
                        const auto direction = directionVectors[path.back()];
                        path.pop_back();

                        if (dungeonPtr && dungeonPtr->canPass(int2(static_cast<int>(position.x) + direction.x, static_cast<int>(position.y) + direction.y)))
                        {
                            position.x += direction.x;
                            position.y += direction.y;
                        }
                    }
                }
                break;

                default:
                break;
            }
        }
    }
};