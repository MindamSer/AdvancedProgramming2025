#include <SDL3/SDL.h>

#include "world.h"
#include "optick.h"

#include <iostream>
#include <memory>


void init_world(SDL_Renderer* renderer, World& world);
void render_world(SDL_Window* window, SDL_Renderer* renderer, World& world);

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Advanced Programming Course - Androsov Artem",
        1600, 1200,
        SDL_WINDOW_RESIZABLE // вместо SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    {
        auto world = std::make_shared<World>();
        {
            OPTICK_EVENT("world.init");
            // Инициализация всех игровых объектов
            init_world(renderer, *world);
        }

        bool quit = false;
        SDL_Event e;

        Uint64 lastFrameTicks;
        Uint64 currFrameTicks;
        float deltaTime;

        lastFrameTicks = SDL_GetTicks();
        while (!quit) {
	        OPTICK_FRAME("MainThread");

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }

            // Обновление времени
            currFrameTicks = SDL_GetTicks();
            deltaTime = (currFrameTicks - lastFrameTicks) / 1000.0f;
            lastFrameTicks = currFrameTicks;

            {
                OPTICK_EVENT("world.update");
                // Обновление всех игровых объектов
                world->update(deltaTime);
            }

            SDL_SetRenderDrawColor(renderer, 50, 50, 150, 255);
            SDL_RenderClear(renderer);
            {
                OPTICK_EVENT("world.render");
                // Отрисовка всех игровых объектов
                render_world(window, renderer, *world);
            }
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}