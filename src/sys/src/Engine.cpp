// Engine.cpp
#include "Singleton.h"
#include <cstring>

extern void parseTextureAtlas(const char* filename, TextureAtlas& atlas, std::vector<Sprite>& sprites);
std::vector<Sprite> sprites;

SDL_Texture* Engine::loadTexture(const char* filePath)
{
    std::string patha(filePath);
    if (strncmp(filePath, "data/", 5) != 0) {
        patha = "data/" + patha;
    }
    const char* path = patha.c_str();    

    SDL_Surface* surface = nullptr;
    
    if (strstr(path, ".png")) {
        std::string newPath(path);
        size_t pos = newPath.rfind(".png");

        if (pos != std::string::npos) {
            newPath.replace(pos, 4, ".avif");

            LoaderAVIF loader;
            surface = loader.load(newPath);
        }
    } else if (strstr(path, ".avif")) {
        LoaderAVIF loader;
        surface = loader.load(path);
    } else {
        std::cerr << "Unsupported file format: " << path << std::endl;
        return nullptr;
    }

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

const Sprite* Engine::getButton(int mouseX, int mouseY, const std::vector<Sprite>& buttons)
{
    for (const auto& button : buttons) {
        if (mouseX >= button.x && mouseX <= button.x + button.width &&
            mouseY >= button.y && mouseY <= button.y + button.height) {
            return &button;
        }
    }
    return nullptr;
}

void Engine::mainLoop()
{
    bool quit = false;
    SDL_Event e;

    std::vector<Sprite> sprites;

    SDL_Texture* loggingin = loadTexture("data/gfx/menu/logging_in.avif");
    SDL_Texture* bbblogo = loadTexture("data/gfx/menu/BBB_logo_loading_screen.avif");
    SDL_Rect bbblogorect = { 0, 0, 160, 200 };
    SDL_Texture* cmflogo = loadTexture("data/gfx/menu/credits_CMF_logo.avif");
    SDL_Rect cmflogorect = { 1120, 0, 160, 40 };
    TextureAtlas atlas;
    parseTextureAtlas("data/xml_resources/buttons01.xml", atlas, sprites);
    SDL_Texture* playbutton = loadTexture(atlas.imagePath.c_str());
    SDL_Rect buttonRect = { 0, 0, 64, 78 };
    SDL_Rect buttonRect2;

    bool buttonHeld = false;
    Sprite* buttonSprite = nullptr;

    for (auto& sprite : sprites) {
        if (sprite.name == "button_invite") {
            buttonSprite = &sprite;
            buttonRect2 = { 
                sprite.x,
                sprite.y,
                sprite.width,
                sprite.height 
            };

            buttonRect.x = (screenWidth - sprite.width) / 2;
            buttonRect.y = (screenHeight - sprite.height) * 0.95;
            buttonRect.w = sprite.width;
            buttonRect.h = sprite.height;
            break;
        }
    }

    while (!quit) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, loggingin, nullptr, nullptr);
        SDL_RenderCopy(renderer, bbblogo, nullptr, &bbblogorect);
        SDL_RenderCopy(renderer, cmflogo, nullptr, &cmflogorect);
        SDL_RenderCopy(renderer, playbutton, &buttonRect2, &buttonRect);

        SDL_RenderPresent(renderer);
    }
}
