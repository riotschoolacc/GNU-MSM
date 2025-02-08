// Engine.cpp
#include "EngineSDL.cpp"

int main(int argc, char *argv[])
{
    EngineSDL engine(800, 600);

    if (!engine.init()) {
        return -1;
    }

    engine.mainLoop();

    return 0;
}