#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <string>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

/* GLOBALS */
const int COLUMNS = 120;
const int WIDTH = COLUMNS*10;
const int HEIGHT = 600;

const double unit = (double)HEIGHT/COLUMNS;

SDL_Event ev;

/* GLOBAL FUNCTIONS */
Uint32 timer(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

/* GRAPH CLASS */
class Graph {
    int array[COLUMNS];
    std::set<int> special;
    std::set<int> sorted;
    std::string text;
    int current;
    SDL_Renderer *graphRenderer;
public:
    Graph(SDL_Renderer *renderer)
    : current(-1)
    , graphRenderer(renderer)
    {
        for(int i = 0; i < COLUMNS; i++)
            array[i] = i+1;
    }

    void draw() const {
        boxColor(graphRenderer, 0, 0, WIDTH, HEIGHT, 0x0000FF);
        for(int i = 0; i < COLUMNS; i++) {
            // current
            if(i == current)
                boxRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 255, 40, 40, 255);
            // sorted 
            else if(sorted.find(i) != sorted.end()) 
                boxRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 0, 255, 0, 255);
            // special
            else if(special.find(i) != special.end())
                boxRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 0, 150, 255, 255);
            // all others
            else
                boxRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 200, 200, 200, 255);
            
            rectangleRGBA(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 255, 255, 255, 255);
        }
        SDL_RenderPresent(graphRenderer);
    }
    
    void swap_randomly() {
        int ind1 = rand()%COLUMNS;
        int ind2 = rand()%COLUMNS;
        
        int temp = array[ind1];
        array[ind1] = array[ind2];
        array[ind2] = temp;
    }
    
    void scramble() {
        for(int i = 1; i <= COLUMNS*2; i++)
            swap_randomly();
        update_text("Array scrambled.");
    }
    
    bool is_sorted() {
        for(int i = 0; i < COLUMNS-1; i++)
            if(array[i] >= array[i+1])
                return false;
        return true;
    }
    
    void update_text(const char* str) {
        text = str;
        // Draw a black box over previous text (with 1 px padding on top and bottom)
        // Text is 12px high
        boxColor(graphRenderer, 0, HEIGHT+8-1, WIDTH, HEIGHT+8+12+1, 0x000000FF);
        // Display the updated text
        stringRGBA(graphRenderer, 4, HEIGHT+8, text.c_str(), 255, 255, 255, 255);
        // Commit changes
        SDL_RenderPresent(graphRenderer);
    }
    
    void bubble_sort() {        
        update_text("Running bubble sort...");
        SDL_TimerID id = SDL_AddTimer(2, timer, NULL);
        
        int i, j;
     
        for (i = COLUMNS-1; i > 0; --i) {
            for (j = 0; j < i; ++j) {
                current = j;
                
                while(SDL_WaitEvent(&ev) && ev.type != SDL_USEREVENT);
                draw();
                
                if (array[j+1] < array[j]) {
                    int temp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = temp;
                }
                
                sorted.insert(i);
            }
        }
        
        SDL_RemoveTimer(id);

        sorted.insert(0);
        sorted.insert(1);
        draw();
        
        update_text("Bubble sort done!");
    }

    void selection_sort() {
        update_text("Running selection sort...");
        SDL_TimerID id = SDL_AddTimer(5, timer, NULL);
        
        int i, j, minindex;
        
        for (i = 0; i < COLUMNS-1; ++i) {
            minindex = i;
            special.insert(i);
            for (j = i+1; j < COLUMNS; ++j) {
                current = j;
                if (array[j] < array[minindex]) {
                    minindex = j;
                    special.clear();
                    special.insert(j);
                }
                
                while(SDL_WaitEvent(&ev) && ev.type != SDL_USEREVENT);
                draw();
            }
            if (minindex != i) {
                int temp = array[minindex];
                array[minindex] = array[i];
                array[i] = temp;
            }
            special.clear();
            sorted.insert(i);
        }
       
        SDL_RemoveTimer(id);
        
        sorted.insert(COLUMNS-2);
        sorted.insert(COLUMNS-1);
        draw();
        
        update_text("Selection sort done!");
    }

    void gnome_sort() {
        update_text("Running gnome sort...");
        SDL_TimerID id = SDL_AddTimer(5, timer, NULL);
        
        int i = 0;
        
        while (i < COLUMNS) {
            current = i;
            
            if (i == 0 || array[i-1] <= array[i]) {
                current = -1;
                i++;
                sorted.insert(i);
            }
            else {
                int tmp = array[i];
                array[i] = array[i-1];
                array[i-1] = tmp;
                i--;
                current = i;
            }

            while(SDL_WaitEvent(&ev) && ev.type != SDL_USEREVENT);
            draw();
        }
        
        SDL_RemoveTimer(id);
        
        sorted.insert(0);
        draw();
        
        update_text("Gnome sort done!");
    }

    void bogo_sort() {
        update_text("Running bogo sort... (This may take a while, please be patient.)");
        SDL_TimerID id = SDL_AddTimer(20, timer, NULL);
        
        while(!is_sorted()) {
            scramble();
            
            // Add some colour to it
            if(rand()%10)
                special.insert(rand()%COLUMNS);
            // But not too much
            else
                special.clear();
                
            while(SDL_WaitEvent(&ev) && ev.type != SDL_USEREVENT);
            draw();
        }
        
        SDL_RemoveTimer(id);
        
        for(int i = 0; i < COLUMNS; i++)
            sorted.insert(i);
        draw();
        
        // Celebrate improbability
        update_text("Wow. Would you look at that. Bogo sort done.");
    }
    
    
};

/* MAIN */
int main(int argc, char *argv[]) {
    srand(time(0));
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    try {
        sdlWindow = SDL_CreateWindow("Sort visualizer",
                                 SDL_WINDOWPOS_UNDEFINED,    // Alternatively, use SDL_WINDOWPOS_CENTERED here
                                 SDL_WINDOWPOS_UNDEFINED,
                                 WIDTH, HEIGHT+20,
                                 0);
        if(sdlWindow == NULL) throw 1;
        
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
        if(sdlRenderer == NULL) throw 1;
    }
    catch (int error) {
        SDL_Quit();
        return error;
    }

    // Set black background as a start, just to be sure
    boxColor(sdlRenderer, 0, 0, WIDTH, HEIGHT, 0x000000FF);
    SDL_RenderPresent(sdlRenderer);    
    
    Graph g(sdlRenderer);
    g.update_text("Sort visualizer loaded, please choose an option from below!");
    g.draw();
    
    bool quit = false;
    while(SDL_WaitEvent(&ev) && !quit) {
        switch(ev.type) {
            case SDL_QUIT:
                quit = true;
            break;
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                    break;
                    case SDLK_BACKQUOTE:
                    case SDLK_0:
                    case SDLK_KP_0:
                        g.scramble();
                        g.draw();
                    break;
                    case SDLK_1:
                    case SDLK_KP_1:
                        g.selection_sort();
                    break;
                    case SDLK_2:
                    case SDLK_KP_2:
                        g.bubble_sort();
                    break;
                    case SDLK_3:
                    case SDLK_KP_3:
                        g.gnome_sort();
                    break;
                    case SDLK_4:
                    case SDLK_KP_4:
                        g.bogo_sort();
                    break;
                }
            break;
        }
    }
    
    SDL_Quit();
    return 0;
}
