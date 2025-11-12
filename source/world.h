#pragma once

#include "archetypes/entity.h"
#include "archetypes/tile.h"

#include "components/camera2d.h"

#include "systems/moveSystem.h"

#include "dungeon.h"
#include "tileset.h"

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

    MoveSystem moveSys;
};
