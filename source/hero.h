#pragma once

#include <SDL3/SDL_keyboard.h>

#include "components/transform2d.h"
#include "components/dungeon_restrictor.h"
#include "components/sprite.h"
#include "components/camera2d.h"
#include "components/bar.h"

#include "math2d.h"


struct Hero
{
public:
    Hero(int maxHealth, int maxStamina, Camera2D *camera = nullptr)
        : health(maxHealth), stamina(maxStamina), mainCamera(camera)
    {
        if (mainCamera)
            mainCamera->position = position;
    }

    void update(float dt)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        const float cellPerSecond = stamina.getSpeed();

        int2 move;
        bool moved = false;
        if (keys[SDL_SCANCODE_W]) { move.y -= 1; moved = true; }
        if (keys[SDL_SCANCODE_S]) { move.y += 1; moved = true; }
        if (keys[SDL_SCANCODE_A]) { move.x -= 1; moved = true; }
        if (keys[SDL_SCANCODE_D]) { move.x += 1; moved = true; }
        if (!moved)
            return;

        if (moved && (timeSinceLastMode < 1.f / cellPerSecond))
        {
            timeSinceLastMode += dt;
            return;
        }
        timeSinceLastMode = 0.f;

        if (restrictor.canPass(int2(static_cast<int>(position.x) + move.x, static_cast<int>(position.y) + move.y)))
        {
            position.x += move.x;
            position.y += move.y;
            if (mainCamera)
                mainCamera->position = position;
        }
    }

    void bindDungeon(const Dungeon &dungeon) { restrictor.bindDungeon(&dungeon); }
    void clearDungeon() { restrictor.clearDungeon(); }

    void bindCamera(Camera2D &camera)
    {
        mainCamera = &camera;
        camera.position = position;
    }
    void clearCamera() { mainCamera = nullptr; }

public:
    Transform2D position;
    DungeonRestrictor restrictor;
    Sprite sprite;

    HealthBar health;
    StaminaBar stamina;

    Camera2D *mainCamera;
    float timeSinceLastMode = 0.f;
};
