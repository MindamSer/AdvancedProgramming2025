#pragma once

#include "archetypes/entity.h"
#include "archetypes/tile.h"

#include "components/camera2d.h"

#include "hero.h"
#include "tileset.h"
#include "dungeon_generator.h"

#include <memory>


struct World
{
public:
    void init(SDL_Renderer* renderer);
    void update(float dt);
    void render(SDL_Window* window, SDL_Renderer* renderer);

public:
    Camera2D mainCamera;
    std::shared_ptr<Dungeon> dungeon;

    std::unique_ptr<TileSet> tileset;
    tileArchetype backgroundTiles;

    std::unique_ptr<Hero> mainHero;
    entityArchetype enemies;
};
