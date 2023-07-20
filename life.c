#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

enum ERR_CODES {
    INIT_ERR = 10,
};

#define FPS 60

#define WIN_H 500
#define WIN_W 1000
 
int main(int argc, char *argv[]) {
    int status = 0;

    int Win_width = WIN_W;
    int Win_height = WIN_H;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[ERR] Initializing SDL: %s\n", SDL_GetError());
        status = INIT_ERR;
    }

    SDL_Window* window = SDL_CreateWindow("Move Rectangle", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       Win_width,
                                       Win_height,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, renderer_flags);

    if (window == NULL || rend == NULL) {
        printf("[ERR] Initializing window or renderer: %s\n", SDL_GetError());
        status = INIT_ERR;
    }

    int box_size = 3;
    SDL_Rect rect;
    rect.w = box_size;
    rect.h = box_size;
    //rect.x = (Win_width / 2) - (rect.w / 2);
    //rect.y = (Win_height / 2) - (rect.h / 2);


    SDL_Event event;
    int work = 1;
    while (work == 1 && status == 0) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                work = 0;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    work = 0;

                }
            }
        }

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        rect.x = (Win_width / 2) - (rect.w / 2);
        rect.y = (Win_height / 2) - (rect.h / 2);

        SDL_RenderClear(rend); // clear all screen

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // set drawing color for rect

        //SDL_RenderDrawRect(rend, &rect); // draw rect
        //SDL_RenderFillRect(rend, &rect); // draw rect
        SDL_RenderDrawPoint(rend, (Win_width / 2), (Win_height / 2));

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); // set color for screen

        SDL_RenderPresent(rend); // apply previous renderer call

        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return status;
}
