// Singleton.h
#ifndef SINGLETON_H
#define SINGLETON_H

#include "EngineSDL.cpp"
#include "LoaderAVIF.cpp"

class Engine: public EngineSDL {
public:
    Engine(int screenWidth, int screenHeight) : EngineSDL(screenWidth, screenHeight) {}
    
    bool init() override;
    void mainLoop() override;
    void cleanup() override;
    
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif