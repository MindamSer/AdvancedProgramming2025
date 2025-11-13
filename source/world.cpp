#include "world.h"
#include "dungeon.h"

#include <iostream>
#include <memory>


constexpr int LevelWidth = 120;
constexpr int LevelHeight = 50;
constexpr int RoomAttempts = 100;

constexpr int BotPopulationCount = 100;
constexpr float PredatorProbability = 0.2f;
constexpr int InitialFoodAmount = 100;

constexpr int SotfMaxEntityCount = BotPopulationCount * 4;
constexpr int SotfMaxFoodCount = InitialFoodAmount * 4;


void World::init(SDL_Renderer* renderer)
{
    // Загружаем текстуру с тайлами
    TexturePtr tilemap = LoadTextureFromFile("assets/kenney_tiny-dungeon/Tilemap/tilemap.png", renderer);
    if (!tilemap)
    {
        std::cerr << "Failed to load tilemap texture\n";
        return;
    }
    tileset = std::make_unique<TileSet>(tilemap);

    // creating camera and dungeon
    mainCamera = Camera2D(0.f, 0.f, 32.f);
    dungeon = std::make_unique<Dungeon>(LevelWidth, LevelHeight, RoomAttempts);

    const auto &dungeonGrid = dungeon->getGrid();
    const size_t tileCount = dungeonGrid.getTileCount();

    // filling background tiles based on dungeon layout
    {
        backgroundTiles.reserve(tileCount);

        for (int y = 0; y < LevelHeight; ++y)
            for (int x = 0; x < LevelWidth; ++x)
            {
                const char * spriteName = nullptr;
                if (dungeonGrid.getTile(x, y) == Dungeon::FLOOR)
                    spriteName = rand() % 2 == 0 ? "floor1" : "floor2";
                else if (dungeonGrid.getTile(x, y) == Dungeon::WALL)
                    spriteName = "wall";

                if (spriteName)
                {
                    backgroundTiles.sprites.push_back(tileset->get_tile(spriteName));
                    backgroundTiles.positions.push_back(Transform2D(x, y));
                }
            }
    }



    // creating entities
    {
        entities.reserve(SotfMaxEntityCount);

        // creating hero
        const auto heroPos = dungeon->getRandomFloorPosition();
        mainCamera.position = heroPos;
        entities.add(
            heroPos,
            tileset->get_tile("knight"),
            dungeon.get(),
            {100},
            {100},
            false,
            false,
            &mainCamera
        );

        // creating npcs
        for (int i = 0; i < BotPopulationCount; ++i)
        {
            const bool isPredator = (rand() % 100) < int(PredatorProbability * 100.f);

            entities.add(
                dungeon->getRandomFloorPosition(),
                isPredator ? tileset->get_tile("ghost") : tileset->get_tile("peasant"),
                dungeon.get(),
                {100},
                {100},
                true,
                isPredator
            );
        }
    }

    // creating food generators
    {
        foods.reserve(SotfMaxFoodCount);

        // for now just make some food for test
        for (size_t i = 0; i < InitialFoodAmount*2; ++i)
        {
            const bool isLarge = rand() % 2;
            const bool isHeath = rand() % 2;

            const int val = isLarge ? 50 : 25;
            Food foodProp = isHeath ? Food{HeathFood{val}} : Food{StaminaFood{val}};
            const char *spriteName = isHeath ? (isLarge ? "health_large" : "health_small") : (isLarge ? "stamina_large" : "stamina_small");

            foods.add(dungeon->getRandomFloorPosition(), tileset->get_tile(spriteName), foodProp);
        }
    }
}

void World::update(float dt)
{
    entities.performDeletion();
    foods.performDeletion();

    moveSys.process(dt, entities);
    starveSys.process(dt, entities);
    tiredSys.process(dt, entities);
    predSys.process(dt, entities);

    foodSys.process(dt, entities, foods);
}

constexpr float GREY[4] = {0.2f, 0.2f, 0.2f, 1.f};
constexpr float HEALTH_COLOR[4] = {0.91f, 0.27f, 0.22f, 1.f};
constexpr float STAMINA_COLOR[4] = {0.f, 0.6f, 0.86f, 1.f};

void World::render(SDL_Window* window, SDL_Renderer* renderer)
{
    int screenW, screenH;
    SDL_GetWindowSize(window, &screenW, &screenH);

    // Draw background sprites
    {
        for (size_t i = 0; i < backgroundTiles.positions.size(); ++i)
        {
            SDL_FRect dst = mainCamera.toCameraSpace(backgroundTiles.positions[i]);
            dst.x += screenW / 2.f;
            dst.y += screenH / 2.f;
            draw_strite(renderer, backgroundTiles.sprites[i], dst);
        }
    }

    // Draw foods
    {
        for (size_t i = 0; i < foods.size(); ++i)
        {
            SDL_FRect dst = mainCamera.toCameraSpace(foods.positions[i]);
            dst.x += screenW / 2.f;
            dst.y += screenH / 2.f;
            draw_strite(renderer, foods.sprites[i], dst);
        }
    }

    // Draw entities
    {
        // draw sprites
        for (size_t i = 0; i < entities.size(); ++i)
        {
            SDL_FRect dst = mainCamera.toCameraSpace(entities.positions[i]);
            dst.x += screenW / 2.f;
            dst.y += screenH / 2.f;
            draw_strite(renderer, entities.sprites[i], dst);
        }

        // draw bars
        {
            static std::vector<SDL_FRect> backBars(SotfMaxEntityCount);
            static std::vector<SDL_FRect> healthBars(SotfMaxEntityCount);
            static std::vector<SDL_FRect> staminaBars(SotfMaxEntityCount);

            for (size_t i = 0; i < entities.size(); ++i)
            {
                const auto entityPos = entities.positions[i];
                const auto entityHeath = entities.healthBars[i];
                const auto entityStamina = entities.staminaBars[i];


                Transform2D healthBarTransform = entityPos;
                healthBarTransform.sizeX *= 0.1f;
                SDL_FRect healthDst = mainCamera.toCameraSpace(healthBarTransform);
                healthDst.x += screenW / 2.f;
                healthDst.y += screenH / 2.f;
                backBars.push_back(healthDst);

                const float healthFrac = float(entityHeath.getValue()) / float(entityHeath.getMax());
                healthDst.y += (1.f - healthFrac) * healthDst.h;
                healthDst.h *= healthFrac;
                healthBars.push_back(healthDst);


                Transform2D staminaBarTransform = entityPos;
                staminaBarTransform.x += staminaBarTransform.sizeX * 0.9f;
                staminaBarTransform.sizeX *= 0.1f;
                SDL_FRect staminaDst = mainCamera.toCameraSpace(staminaBarTransform);
                staminaDst.x += screenW / 2.f;
                staminaDst.y += screenH / 2.f;
                backBars.push_back(staminaDst);

                const float staminaFrac = float(entityStamina.getValue()) / float(entityStamina.getMax());
                staminaDst.y += (1.f - staminaFrac) * staminaDst.h;
                staminaDst.h *= staminaFrac;
                staminaBars.push_back(staminaDst);
            }

            SDL_SetRenderDrawColorFloat(renderer, GREY[0], GREY[1], GREY[2], GREY[3]);
            SDL_RenderFillRects(renderer, backBars.data(), int(backBars.size()));
            SDL_SetRenderDrawColorFloat(renderer, HEALTH_COLOR[0], HEALTH_COLOR[1], HEALTH_COLOR[2], HEALTH_COLOR[3]);
            SDL_RenderFillRects(renderer, healthBars.data(), int(healthBars.size()));
            SDL_SetRenderDrawColorFloat(renderer, STAMINA_COLOR[0], STAMINA_COLOR[1], STAMINA_COLOR[2], STAMINA_COLOR[3]);
            SDL_RenderFillRects(renderer, staminaBars.data(), int(staminaBars.size()));

            backBars.clear();
            healthBars.clear();
            staminaBars.clear();
        }
    }
}
