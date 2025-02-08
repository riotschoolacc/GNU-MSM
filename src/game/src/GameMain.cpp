// GameMain.cpp
#include "../../sys/src/Singleton.h"

int main(int argc, char *argv[]) {
    Engine engine(1280, 720);

    if (!engine.init()) {
        return -1;
    }

    engine.mainLoop();
    engine.cleanup();

    return 0;
}
