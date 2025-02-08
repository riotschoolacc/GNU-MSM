// Engine.cpp
#include "Singleton.h"

bool Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("My Singing Monsters", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Engine::cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::mainLoop()
{
    bool quit = false;
    SDL_Event e;

    LoaderAVIF loader;
    SDL_Surface* surface = loader.load("data/gfx/menu/BBB_logo_loading_screen.avif");
    if (!surface) {
        std::cerr << "Failed to load AVIF image." << std::endl;
        return;
    }

    SDL_Texture* avifTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!avifTexture) {
        std::cerr << "Failed to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        if (avifTexture) {
            SDL_RenderCopy(renderer, avifTexture, nullptr, nullptr);
        }

        SDL_RenderPresent(renderer);
    }
}
