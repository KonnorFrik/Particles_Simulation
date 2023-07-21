#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <math.h>

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
        status = INIT_ERR;
        printf("[ERR %d] Initializing window or renderer: %s\n", status, SDL_GetError());
        exit(status);
    }

    int radius_step = 1;

    int atoms_count = START_ATOMS_COUNT;
    ATOM** atoms_arr = calloc(atoms_count, sizeof(ATOM*));
    if (atoms_arr == NULL) {
        status = NULL_PTR;
        printf("[ERR %d] Can't allocate atom array\n", status);
        exit(status);
    }

    // init group 
    for (int i = 0; i < atoms_count; ++i) {
        atoms_arr[i] = init_atom(&status);
    }

    if (status) {
        printf("[ERR %d] Can't allocate atom\n", status);
        exit(status);
    }
    
    // fill group
    for (int i = 0; i < atoms_count; ++i) {
        float* powers = get_powers_by_code(RED);
        fill_atom(atoms_arr[i], START_ATOM_RADIUS, RED, powers);
    }

    random_position(atoms_arr, atoms_count);

    if (DEBUG) {
        printf("[DEBUG] 1st atom\n");
        printf("x:%d y:%d\n", atoms_arr[0]->atom->x, atoms_arr[0]->atom->y);
        printf("color code:%d\n", atoms_arr[0]->color_code);
        printf("powers:%f\n", atoms_arr[0]->powers[0]);
        printf("[DEBUG]\n\n");
        getchar();
    }

    float g = -1.4;

    SDL_Event event;
    int work = 1;
    while (work == 1 && status == 0) {
        SDL_Delay(1000 / FPS);
        SDL_RenderClear(rend); // clear all screen

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                work = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    work = 0;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) {
                    for (int i = 0; i < atoms_count; ++i) {
                        change_radius(atoms_arr[i]->atom, atoms_arr[i]->atom->w + radius_step);
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS) {
                    for (int i = 0; i < atoms_count; ++i) {
                        change_radius(atoms_arr[i]->atom, atoms_arr[i]->atom->w - radius_step);
                    }
                }
            }
        }

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        /***********PROCESSING PART***********/
        //need func for update all atoms in arr with their rule (
        for (int i = 0; i < atoms_count - 1; ++i) {
            float fx = 0;
            float fy = 0;

            float dx = atoms_arr[i]->atom->x - atoms_arr[i + 1]->atom->x;
            float dy = atoms_arr[i]->atom->y - atoms_arr[i + 1]->atom->y;
            int d = sqrt((dx * dx) + (dy * dy));
            if (d > 0) {
                float F = atoms_arr[i]->powers[0] * 1 / d;
                fx += F * dx;
                fy += F * dy;
                if (DEBUG) {
                    printf("F: %f\n", F);
                }
            }

            atoms_arr[i]->atom->x += round(fx);
            atoms_arr[i]->atom->y += round(fy);

            if (DEBUG) {
                printf("[DEBUG]\n");
                printf("fx: %f  fy: %f\n", fx, fy);
                printf("dx: %f  dy: %f\n", dx, dy);
                printf("d: %d\n", d);
                printf("i: %d, atom[i]->x: %d  atom[i]->y: %d\n", i, atoms_arr[i]->atom->x, atoms_arr[i]->atom->y);
                printf("\n");
            }
        }
        /***********PROCESSING PART***********/

        /***********RENDER PART***********/

        ///////need func for draw all atoms in arr with their color
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // set drawing color for rect
        for (int i = 0; i < atoms_count; ++i) {
            SDL_RenderFillRect(rend, atoms_arr[i]->atom); // draw filled rect
        }
        //////////////////////////////////////////////////////////////////////////////

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); // set color for screen
        SDL_RenderPresent(rend); // apply previous renderer call's

    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return status;
}
