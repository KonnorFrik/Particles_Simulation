#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "settings.h"
#include "life.h"

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

    int radius_step = 1;

    SDL_Rect rect;
    rect.w = MIN_ATOM_RADUIS;
    rect.h = MIN_ATOM_RADUIS;

    SDL_Event event;
    int work = 1;
    while (work == 1 && status == 0) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                work = 0;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    work = 0;
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) {
                    change_radius(&rect, rect.w + radius_step);

                }

                if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS) {
                    change_radius(&rect, rect.w - radius_step);
                }
            }
        }

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        /***********PROCESSING PART***********/
        //need func for update all atoms in arr with their rule (
        rect.x = (Win_width / 2) - (rect.w / 2);
        rect.y = (Win_height / 2) - (rect.h / 2);
        /***********PROCESSING PART***********/

        /***********RENDER PART***********/
        SDL_RenderClear(rend); // clear all screen

        ///////need func for draw all atoms in arr with their color
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // set drawing color for rect
        SDL_RenderFillRect(rend, &rect); // draw filled rect
        //////////////////////////////////////////////////////////////////////////////

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); // set color for screen
        SDL_RenderPresent(rend); // apply previous renderer call's

        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return status;
}
