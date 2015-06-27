/* SDL2 C++ sample project */

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

int main(int argc, char *argv[]) {
	SDL_Event ev;
	SDL_Window *sdlWindow;
	SDL_Renderer *sdlRenderer;
	try {
		sdlWindow = SDL_CreateWindow("Title",
								 SDL_WINDOWPOS_UNDEFINED,	// Alternatively, use SDL_WINDOWPOS_CENTERED here
								 SDL_WINDOWPOS_UNDEFINED,
								 440, 360,					// Set both to 0 if you use fullscreen
								 0);						// Change 0 to SDL_WINDOW_FULLSCREEN_DESKTOP for fullscreen
		if(sdlWindow == NULL) throw 1;
		
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
		if(sdlRenderer == NULL) throw 1;
		
		// Uncomment these lines if you want to scale to fullscreen
		// SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
		// SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);
	}
	catch (int error) {
		SDL_Quit();
		return error;
	}
	
	// sets renderer colour to black
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	// paints the screen with the set colour
	SDL_RenderClear(sdlRenderer);
	// submits changes to the screen
	SDL_RenderPresent(sdlRenderer);
		
	int r = 50;

    // circle
    int x = 100; int y = 100;
    circleRGBA(sdlRenderer, x, y, r,                                 255, 0, 0, 255);
    circleRGBA(sdlRenderer, x+r, y, r,                               0, 255, 0, 255);
    circleRGBA(sdlRenderer, x+r*cos(3.1415/3), y-r*sin(3.1415/3), r, 0, 0, 255, 255);

    // antialiased circle
    x = 280; y = 100;
    aacircleRGBA(sdlRenderer, x, y, r,                                 255, 0, 0, 255);
    aacircleRGBA(sdlRenderer, x+r, y, r,                               0, 255, 0, 255);
    aacircleRGBA(sdlRenderer, x+r*cos(3.1415/3), y-r*sin(3.1415/3), r, 0, 0, 255, 255);

    // filled circle
    x = 100; y = 280;
    filledCircleColor(sdlRenderer, x, y, r,                                 0xFF0000FF);
    filledCircleColor(sdlRenderer, x+r, y, r,                               0x00FF00FF);
    filledCircleColor(sdlRenderer, x+r*cos(3.1415/3), y-r*sin(3.1415/3), r, 0x0000FFFF);

    // transparent circle
    x = 280; y = 280;
    filledCircleRGBA(sdlRenderer, x, y, r,                                 255, 0, 0, 96);
    filledCircleRGBA(sdlRenderer, x+r, y, r,                               0, 255, 0, 96);
    filledCircleRGBA(sdlRenderer, x+r*cos(3.1415/3), y-r*sin(3.1415/3), r, 0, 0, 255, 96);

    // text
    stringRGBA(sdlRenderer, 70, 350, "Click the X in the top right to exit", 255, 255, 255, 255);

	// submit changes to the screen
	SDL_RenderPresent(sdlRenderer);
	
	while(SDL_WaitEvent(&ev) && ev.type != SDL_QUIT);
	
	SDL_Quit();
	return 0;
}