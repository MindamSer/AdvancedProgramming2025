#pragma once

#include "archetypes/entity.h"
#include "archetypes/food.h"
#include "archetypes/tile.h"

#include "components/camera2d.h"

#include "systems/move.h"
#include "systems/starvation.h"
#include "systems/tiredness.h"
#include "systems/predator.h"
#include "systems/food.h"

#include "dungeon.h"
#include "tileset.h"
#include "foodGenerator.h"

#include <memory>


struct World
{
public:
    void init(SDL_Renderer* renderer);
    void update(float dt);
    void render(SDL_Window* window, SDL_Renderer* renderer);

public:
    Camera2D mainCamera;
    std::unique_ptr<Dungeon> dungeon;

    std::unique_ptr<TileSet> tileset;
    tileArchetype backgroundTiles;

    EntityArchetype entities;
    FoodArchetype foods;

    MoveSystem moveSys;
    StarvationSystem starveSys;
    TirednessSystem tiredSys;
    PredatorSystem predSys;

    std::unique_ptr<FoodGenerator> foodGenerator;
    FoodSystem foodSys;
};
