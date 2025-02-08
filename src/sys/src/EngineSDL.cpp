#include <SDL2/SDL.h>

extern void gameLoop();

int main( int argc, char* argv[] ) {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow( "My Singing Monsters",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           800, 600,
                                           SDL_WINDOW_SHOWN );
    if( !window ) {
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int running = 1;
    while( running ) {
        while( SDL_PollEvent( &event ) ) {
            gameLoop();
            if( event.type == SDL_QUIT ) {
                running = 0;
            }
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
