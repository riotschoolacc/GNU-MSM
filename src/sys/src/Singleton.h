// Singleton.h
#ifndef SINGLETON_H
#define SINGLETON_H

#include "EngineSDL.cpp"
#include "LoaderAVIF.cpp"
#include "LoaderPNG.cpp"

struct Sprite {
    std::string name;
    int x, y, width, height;
    float pivotX, pivotY;
    int offsetX, offsetY;
    int offsetWidth, offsetHeight;
    bool rotated;
};

struct TextureAtlas {
    std::string imagePath;
    int width, height;
};

class Engine: public EngineSDL {
public:
    Engine(int screenWidth, int screenHeight) : EngineSDL(screenWidth, screenHeight) {}
    
    bool init() override;
    void mainLoop() override;
    void cleanup() override;

    SDL_Texture* loadTexture(const char* filePath);
    const Sprite* getButton(int mouseX, int mouseY, const std::vector<Sprite>& buttons);

protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif