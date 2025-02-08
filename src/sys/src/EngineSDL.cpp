// EngineSDL.cpp
#include <SDL2/SDL.h>
#include <iostream>

class EngineSDL {
public:
    EngineSDL(int width, int height) 
    : window(nullptr), renderer(nullptr), screenWidth(width), screenHeight(height) {}

    virtual ~EngineSDL() = default;

    virtual bool init() = 0;
    virtual void mainLoop() = 0;

    virtual void cleanup() = 0;

protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
};
