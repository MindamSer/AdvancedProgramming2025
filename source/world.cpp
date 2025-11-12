#include "world.h"

#include <iostream>


constexpr int LevelWidth = 120;
constexpr int LevelHeight = 50;
constexpr int RoomAttempts = 100;
constexpr int BotPopulationCount = 100;
constexpr float PredatorProbability = 0.2f;
constexpr int InitialFoodAmount = 100;


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
    dungeon = std::make_shared<Dungeon>(LevelWidth, LevelHeight, RoomAttempts);
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

    // creating main hero
    {
        const auto heroPos = dungeon->getRandomFloorPosition();

        mainHero = std::make_unique<Hero>(100, 100);
        mainHero->position = Transform2D(heroPos.x, heroPos.y);
        mainHero->sprite = tileset->get_tile("knight");
        mainHero->bindCamera(mainCamera);
        mainHero->bindDungeon(*dungeon);
    }

    // creating npcs
    {
        enemies.reserve(BotPopulationCount);

        for (int i = 0; i < BotPopulationCount; ++i) {
            const bool isPredator = (rand() % 100) < int(PredatorProbability * 100.f);
            const auto enemyPos = dungeon->getRandomFloorPosition();

            enemies.addEntity(
                Transform2D(enemyPos.x, enemyPos.y),
                DungeonRestrictor(dungeon.get()),
                isPredator ? tileset->get_tile("ghost") : tileset->get_tile("peasant"),
                HealthBar(100),
                StaminaBar(100),
                isPredator
            );
        }
    }


    // auto foodFabriques = create_food_fabriques(world, tileset);
    // auto foodGenerator = world.create_object();
    // auto generatorComp = foodGenerator->add_component<FoodGenerator>(dungeon, std::move(foodFabriques), 2.f / RoomAttempts);
    // for (int i = 0; i < InitialFoodAmount; i++)
    //     generatorComp->generate_random_food();
    // auto starvation = world.create_object();
    // starvation->add_component<StarvationSystem>();
    // auto tiredness = world.create_object();
    // tiredness->add_component<TirednessSystem>();
}

void World::update(float dt)
{
    mainHero->update(dt);

    enemies.performDeletion();
    for (size_t i = 0; i < enemies.positions.size(); ++i)
    {
        auto &position = enemies.positions[i];
        auto &restrictor = enemies.restrictors[i];
        auto &accumulatedTime = enemies.accumulatedTimes[i];
        auto &stamina = enemies.staminaBars[i];

        accumulatedTime += dt * stamina.getSpeed();
        if (accumulatedTime < 1.0f)
            continue;
        accumulatedTime -= 1.0f;

        const int2 directions [] = {
            int2{1,0},
            int2{-1,0},
            int2{0,1},
            int2{0,-1}
        };
        int2 move = directions[rand() % 4];

        if (restrictor.canPass(int2(static_cast<int>(position.x) + move.x, static_cast<int>(position.y) + move.y)))
        {
            position.x += move.x;
            position.y += move.y;
        }
    }
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

    // Draw foreground sprites
    {

    }

    // Draw entities
    {
        // draw sprites
        for (size_t i = 0; i < enemies.positions.size(); ++i)
        {
            SDL_FRect dst = mainCamera.toCameraSpace(enemies.positions[i]);
            dst.x += screenW / 2.f;
            dst.y += screenH / 2.f;
            draw_strite(renderer, enemies.sprites[i], dst);
        }
        {
            SDL_FRect dst = mainCamera.toCameraSpace(mainHero->position);
            dst.x += screenW / 2.f;
            dst.y += screenH / 2.f;
            draw_strite(renderer, mainHero->sprite, dst);
        }

        // draw bars
        {
            static std::vector<SDL_FRect> backBars(BotPopulationCount + 1);
            static std::vector<SDL_FRect> healthBars(BotPopulationCount + 1);
            static std::vector<SDL_FRect> staminaBars(BotPopulationCount + 1);

            {
                Transform2D healthBarTransform = mainHero->position;
                healthBarTransform.sizeX *= 0.1f;
                SDL_FRect healthDst = mainCamera.toCameraSpace(healthBarTransform);
                healthDst.x += screenW / 2.f;
                healthDst.y += screenH / 2.f;
                backBars.push_back(healthDst);

                const float healthFrac = float(mainHero->health.getValue()) / float(mainHero->health.getMax());
                healthDst.y += (1.f - healthFrac) * healthDst.h;
                healthDst.h *= healthFrac;
                healthBars.push_back(healthDst);


                Transform2D staminaBarTransform = mainHero->position;
                staminaBarTransform.x += staminaBarTransform.sizeX * 0.9f;
                staminaBarTransform.sizeX *= 0.1f;
                SDL_FRect staminaDst = mainCamera.toCameraSpace(staminaBarTransform);
                staminaDst.x += screenW / 2.f;
                staminaDst.y += screenH / 2.f;
                backBars.push_back(staminaDst);

                const float staminaFrac = float(mainHero->stamina.getValue()) / float(mainHero->stamina.getMax());
                staminaDst.y += (1.f - staminaFrac) * staminaDst.h;
                staminaDst.h *= staminaFrac;
                staminaBars.push_back(staminaDst);
            }
            for (size_t i = 0; i < enemies.positions.size(); ++i)
            {
                const auto &entityPos = enemies.positions[i];
                const auto &entityHeath = enemies.healthBars[i];
                const auto &entityStamina = enemies.staminaBars[i];


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
