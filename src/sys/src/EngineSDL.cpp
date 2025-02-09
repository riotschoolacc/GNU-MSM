// EngineSDL.cpp
#include <SDL2/SDL.h>
#include "Singleton.h"
#include <iostream>

sys::EngineSDL::EngineSDL(const int width, const int height)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    SDL_Window* window = SDL_CreateWindow("My Singing Monsters",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          width, height,
                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_Texture* texture = loadTexture(renderer, "data/gfx/menu/logging_in.avif");

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}

sys::EngineSDL::~EngineSDL(void)
{
    delete[](this);
    return;
}