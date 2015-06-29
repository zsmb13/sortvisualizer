/* 
 * Sort visualizer by zsmb13@sch.bme.hu
 * 
 * Selection, bubble and gnome sort from
 * https://infoc.eet.bme.hu/ea07/
 * 
 * Heap sort from
 * http://www.bogotobogo.com/Algorithms/heapsort.php
 *
 */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <string>
#include <SDL.h>
#include "SDL2_gfxPrimitives.h"

/* GLOBALS */
const int COLUMNS = 94; // suggested 94-190
const int WIDTH = COLUMNS*10;
const int HEIGHT = 720;

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
    std::string menu_text;
    int current;
    SDL_Renderer *graphRenderer;
    SDL_TimerID id;
public:
    Graph(SDL_Renderer *renderer)
    : current(-1)
    , graphRenderer(renderer)
    {
        for(int i = 0; i < COLUMNS; i++)
            array[i] = i+1;
        
        // Set menu bar strings here
        std::string options[] =  {  "(0) Scramble",
                                    "(1) Selection sort",
                                    "(2) Bubble sort",
                                    "(3) Gnome sort",
                                    "(4) Bogo sort",
                                    "(5) Heap sort",
                                    "(ESC) Quit",
                                    };
        for(auto x : options)
            menu_text += x + ' ';
        
        draw();
        draw_menu();
        update_text("");
    }
    
    ~Graph() {
        SDL_RemoveTimer(id);
    }
    
    void delay() {
        while(SDL_WaitEvent(&ev) && ev.type != SDL_USEREVENT) {
            // Using an exception to get back to the main event loop
            // Not a nice solution at all
            if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE) {
                SDL_RemoveTimer(id);
                update_text("Sort cancelled, please choose an option from below!");
                reset();
                draw();
                draw_menu();
                throw 911; // This really could be just any integer number at all
            }
        }
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
            
            // outline
            rectangleColor(graphRenderer, i*10, HEIGHT, (i+1)*10, HEIGHT-array[i]*unit, 0xFFFFFFFF);
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
    
    void reset() {
        sorted.clear();
        special.clear();
        current = -1;
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
        // Draw a black box over previous text
        // Text is 12px high
        boxColor(graphRenderer, 0, HEIGHT, WIDTH, HEIGHT+19, 0x000000FF);
        // Display the updated text
        stringRGBA(graphRenderer, 4, HEIGHT+8, text.c_str(), 255, 255, 255, 255);
        // Commit changes
        SDL_RenderPresent(graphRenderer);
    }
    
    void draw_menu(int sorting = 0) {
        // Draw black box over previous menu
        boxColor(graphRenderer, 0, HEIGHT+21, WIDTH, HEIGHT+40, 0x000000FF);
        // Display the appropriate string
        stringRGBA(graphRenderer, 4, HEIGHT+28, sorting ? "(SPACE) Cancel" : menu_text.c_str(), 255, 255, 255, 255);
        // Redraw separator line
        lineColor(graphRenderer, 0, HEIGHT+20, WIDTH, HEIGHT+20, 0xFFFFFF88);
        // Commit changes
        SDL_RenderPresent(graphRenderer);
    }
    
    /* SORT ALGORITHMS */    
    void selection_sort() {
        update_text("Running selection sort...");
        id = SDL_AddTimer(5, timer, NULL);
        
        int i, j, minindex;
        
        for (i = 0; i < COLUMNS-1; ++i) {
            minindex = i; // searching for the minimum
            special.insert(i);
            for (j = i+1; j < COLUMNS; ++j) {
                current = j;
                if (array[j] < array[minindex]) {
                    minindex = j;
                    special.clear();
                    special.insert(j);
                }
                
                delay();
                draw();
            }
            if (minindex != i) { // swap?
                int temp = array[minindex];
                array[minindex] = array[i]; // swap.
                array[i] = temp;
            }
            special.clear();
            sorted.insert(i);
        }
       
        SDL_RemoveTimer(id);
        
        current = -1;
        sorted.insert(COLUMNS-2);
        sorted.insert(COLUMNS-1);
        draw();
        
        update_text("Selection sort done!");
    }
    
    void bubble_sort() {
        update_text("Running bubble sort...");
        id = SDL_AddTimer(5, timer, NULL);
        
        int i, j;
     
        for (i = COLUMNS-1; i > 0; --i) {
            for (j = 0; j < i; ++j) {
                current = j;
                
                delay();
                draw();
                
                if (array[j+1] < array[j]) { // comparison
                    int temp = array[j];
                    array[j] = array[j+1]; // swap
                    array[j+1] = temp;
                }
                
                sorted.insert(i);
            }
        }
        
        SDL_RemoveTimer(id);

        current = -1;
        sorted.insert(0);
        sorted.insert(1);
        draw();
        
        update_text("Bubble sort done!");
    }
    
    void gnome_sort() {
        update_text("Running gnome sort...");
        id = SDL_AddTimer(6, timer, NULL);
        
        int i = 0;
        
        while (i < COLUMNS) {
            current = i;
            
            if (i == 0 || array[i-1] <= array[i]) { // in order?
                i++;
                current = i;
                sorted.insert(i);
            }
            else {
                int tmp = array[i];
                array[i] = array[i-1];
                array[i-1] = tmp;
                i--;
                current = i;
            }
            
            delay();
            draw();
        }
        
        SDL_RemoveTimer(id);
        
        sorted.insert(0);
        draw();
        
        update_text("Gnome sort done!");
    }

    void bogo_sort() {
        update_text("Running bogo sort... (This may take a while, please be patient.)");
        id = SDL_AddTimer(50, timer, NULL);
        
        while(!is_sorted()) {
            for(int i = 1; i <= COLUMNS*2; i++)
                swap_randomly();
            
            // Add some colour to it
            if(rand()%20) {
                current = rand()%COLUMNS;
                special.insert(rand()%COLUMNS);
            }
            // But not too much
            else
                special.clear();
                
            delay();
            draw();
        }
        
        SDL_RemoveTimer(id);
        
        for(int i = 0; i < COLUMNS; i++)
            sorted.insert(i);
        draw();
        
        // Celebrate extreme improbability
        update_text("Wow. Would you look at that. Bogo sort done.");
    }
    
    /* HEAP SORT */
    void swap(int &m, int &n) {
        int tmp;
        tmp = m;
        m = n;
        n = tmp;
    }

    void heap_sort(int N){
        update_text("Running heap sort...");
        id = SDL_AddTimer(24, timer, NULL);
        
        /* heapify */
        for (int k = N/2; k >= 0; k--) {
            siftDown(k, N);
        }
                
        while (N-1 > 0) {
            /* swap the root(maximum value) of the heap
            with the last element of the heap */
            swap(array[N-1], array[0]);
            sorted.insert(N-1);
            draw();
            
            /* put the heap back in max-heap order */
            siftDown(0, N-1);
            /* N-- : decrease the size of the heap by one
            so that the previous max value will
            stay in its proper placement */
            N--;
            
            delay();
            draw();
        }
        
        SDL_RemoveTimer(id);
        
        current = -1;
        sorted.insert(0);
        draw();
        
        update_text("Heap sort done!");
    }

    void siftDown(int k, int N){
        while ( k*2 + 1 < N ) {
            current = k;
            
            /* For zero-based arrays, the children are 2*i+1 and 2*i+2 */
            int child = 2*k + 1;
            
            special.insert(child);
            special.insert(child+1);
            
            delay();
            draw();
            
            /* get bigger child if there are two children */
            if ((child + 1 < N) && (array[child] < array[child+1]))
                child++;
            
            special.clear();
            
            if (array[k] < array[child]) {   /* out of max-heap order */
                swap( array[child], array[k] );
                /* repeat to continue sifting down the child now */
                k = child;
            }
            else
                return;
            
            delay();
            draw();
        }
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
                                 WIDTH, HEIGHT+2*20,
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
    
    bool quit = false;
    while(SDL_WaitEvent(&ev) && !quit) {
        try {
            switch(ev.type) {
                case SDL_QUIT:
                    quit = true;
                break;
                case SDL_KEYDOWN:
                    g.draw_menu(1); // draw cancel menu
                    g.reset();
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
                        case SDLK_5:
                        case SDLK_KP_5:
                            g.heap_sort(COLUMNS);
                        break;
                    }
                    g.draw_menu(); // draw regular menu
                break;
            }
        }
        catch(int x) {
            // empty
        }
    }
    
    SDL_Quit();
    return 0;
}
