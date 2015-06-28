#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

const int WIDTH = 1280;
const int HEIGHT = 720;

void buborek(double t[], int db) {
   int i, j;
 
   /* egyre rövidebb tömbrészletek ciklusa */
   for (i = db-1; i > 0; --i)
      /* egymás utáni párok ciklusa */
      for (j = 0; j < i; ++j)
         if (t[j+1] < t[j]) {      /* összehasonlítás */
            double temp = t[j];
            t[j] = t[j+1];         /* csere */
            t[j+1] = temp;
         }
}

void kozvetlen(double t[], int db) {
   int i, j, minindex;
 
   for (i = 0; i < db-1; ++i) {
      minindex = i;                /* minimum keresése */
      for (j = i+1; j < db; ++j)
         if (t[j] < t[minindex])
            minindex = j;
      if (minindex != i) {         /* csere? */
         double temp = t[minindex];
         t[minindex] = t[i];       /* csere. */
         t[i] = temp;
      }
   }
}

void torperendez(double t[], int db) {
    int i = 0;
    while (i < db) {
        if (i == 0 || t[i-1] <= t[i]) { /* jó sorrend? */
            i++;                /* előre */
        } else {
            double tmp = t[i];  /* csere */
            t[i] = t[i-1];
            t[i-1] = tmp;
            i--;                /* vissza */
        }
    }
}

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