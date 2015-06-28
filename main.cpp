#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

const int WIDTH = 1280;
const int HEIGHT = 720;

int main(int argc, char *argv[]) {
    SDL_Event ev;
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    try {
        sdlWindow = SDL_CreateWindow("Title",
                                 SDL_WINDOWPOS_UNDEFINED,	// Alternatively, use SDL_WINDOWPOS_CENTERED here
                                 SDL_WINDOWPOS_UNDEFINED,
                                 WIDTH, HEIGHT,
                                 0);
        if(sdlWindow == NULL) throw 1;
        
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
        if(sdlRenderer == NULL) throw 1;
    }
    catch (int error) {
        SDL_Quit();
        return error;
    }


    boxColor(sdlRenderer, 0, 0, WIDTH, HEIGHT, 0x000000FF);
    SDL_RenderPresent(sdlRenderer);

    bool quit = false;
    while(SDL_WaitEvent(&ev) && !quit) {
        switch(ev.type) {
            case SDL_QUIT:
                quit = true;
            break;
            case SDL_KEYDOWN:
                quit = true;
            break;
        }
    }

    SDL_Quit();
    return 0;
}