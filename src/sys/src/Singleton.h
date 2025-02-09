// Singleton.h
#ifndef SINGLETON_H
#define SINGLETON_H
#ifdef _WIN32
#pragma once
#endif
#include <png.h>
#include <stdint.h>
#include <avif/avif.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

namespace sys
{
class EngineSDL {
public:
    EngineSDL(const int width, const int height);
    ~EngineSDL(void);
};

class Engine: public EngineSDL {
public:
    Engine(const int width, const int height);
    ~Engine(void);
};

class LoaderImage {
public:
    LoaderImage(void);
    ~LoaderImage(void);
    
    void Close();
    bool Open(char *param_1,bool param_2,bool param_3,bool param_4);
};

class LoaderAVIF {
public:
    LoaderAVIF();
    ~LoaderAVIF();
    
    SDL_Surface* Open(const std::string& filePath);
    void Close();
};

class LoaderPNG {
public:
    LoaderPNG(void);
    ~LoaderPNG(void);

    void processPNG(png_struct_def *pngstruct);
    int LoadFromData(char *param_1,long param_2);

    int Open(char *param_1,bool param_2,bool param_3,bool param_4);
    void Close();
};

} // namespace sys

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath)
{
    std::string patha(filePath);
    if (strncmp(filePath, "data/", 5) != 0) {
        patha = "data/" + patha;
    }
    const char* path = patha.c_str();    

    SDL_Surface* surface = nullptr;
    
    sys::LoaderAVIF loader;
    surface = loader.Open(path);

    if (!surface) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
    }
    
    return texture;
}

#endif // SINGLETON_H