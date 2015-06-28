#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

const int COLUMNS = 120;
const int WIDTH = COLUMNS*10;
const int HEIGHT = 800;

const int unit = HEIGHT/COLUMNS;

SDL_Event ev;

class Graph {
    int array[COLUMNS];
    SDL_Renderer *graphRenderer;
public:
    Graph(SDL_Renderer *renderer) : graphRenderer(renderer) {
        for(int i = 0; i < COLUMNS; i++)
            array[i] = i;
    }

    void draw() {
        for(int i = 0; i < COLUMNS; i++) {
            boxRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 200, 200, 200, 255);
        }
        SDL_RenderPresent(graphRenderer);
    }
    
    void scramble() {
        
    }

    /*void update() {
        
    }*/
};


void bubble_sort(int array[], int len) {
    int i, j;
 
    for (i = len-1; i > 0; --i)
        for (j = 0; j < i; ++j)
            if (array[j+1] < array[j]) {
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
}

void selection_sort(int array[], int len) {
    int i, j, minindex;

    for (i = 0; i < len-1; ++i) {
        minindex = i;
        for (j = i+1; j < len; ++j) {
            if (array[j] < array[minindex])
                minindex = j;
            if (minindex != i) {
                int temp = array[minindex];
                array[minindex] = array[i];
                array[i] = temp;
            }
        }
    }
}

void gnome_sort(int array[], int len) {
    int i = 0;
    
    while (i < len) {
        if (i == 0 || array[i-1] <= array[i])
            i++;
        else {
            int tmp = array[i];
            array[i] = array[i-1];
            array[i-1] = tmp;
            i--;
        }
    }
}



int main(int argc, char *argv[]) {
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    try {
        sdlWindow = SDL_CreateWindow("Sort visualizer",
                                 SDL_WINDOWPOS_UNDEFINED,    // Alternatively, use SDL_WINDOWPOS_CENTERED here
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
    
    
    
    Graph g(sdlRenderer);
    g.scramble();
    g.draw();
    
    
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
