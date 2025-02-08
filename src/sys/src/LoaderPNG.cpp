// LoaderPNG.cpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdexcept>

class LoaderPNG {
public:
    LoaderPNG() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        }
    }

    ~LoaderPNG() {
        IMG_Quit();
        SDL_Quit();
    }

    SDL_Surface* load(const std::string& path) {
        SDL_Surface* loadedSurface = IMG_Load(("data/" + path).c_str());
        if (loadedSurface == nullptr) {
            throw std::runtime_error("Unable to load image " + path + "! SDL_image Error: " + std::string(IMG_GetError()));
        }
        return loadedSurface;
    }
};
