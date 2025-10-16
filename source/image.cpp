#include <SDL3/SDL.h>
#include <iostream>

#include "image.h"

// stb_image подключаем отдельно
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TexturePtr LoadTextureFromFile(const char* filename, SDL_Renderer* renderer)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load image: " << filename
                  << " error: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }

    // Создаем SDL_Surface из пикселей stbi
    SDL_Surface* surface = SDL_CreateSurfaceFrom(
        width, height, SDL_PIXELFORMAT_RGBA32,   // формат в stbi (мы принудительно RGBA)
        data, width * 4                          // pitch = ширина * 4 байта
    );

    if (!surface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        stbi_image_free(data);
        return nullptr;
    }

    // Создаем текстуру из surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(surface);
    stbi_image_free(data);
    return TexturePtr(texture, [](SDL_Texture* t) {
        if (t)
        {
            SDL_DestroyTexture(t);
        }
    });
}
