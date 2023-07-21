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

    /*****COLOR MODULE*********/
    const int COLOR_CODES[] = { RED, GREEN };

    int* COLORS[sizeof(COLOR_CODES) / sizeof(COLOR_CODES[0])];
    int RED_COLOR[4] = { 255, 0, 0, 255 };
    int GREEN_COLOR[4] = { 0, 255, 0, 255 };
    COLORS[RED] = RED_COLOR;
    COLORS[GREEN] = GREEN_COLOR;
    /*****COLOR MODULE*********/

    int radius_step = 1;

    int groups = sizeof(COLOR_CODES) / sizeof(COLOR_CODES[0]);
    int atoms_per_group = START_ATOMS_COUNT;
    int atoms_count = groups * atoms_per_group;

    // init all groups arr 
    ATOM*** atoms_arr = calloc(groups, sizeof(ATOM**));
    if (atoms_arr == NULL) {
        status = NULL_PTR;
        printf("[ERR %d] Can't allocate atom array\n", status);
        exit(status);
    }

    // init groups 
    for (int gr = 0; gr < groups; ++gr) {
        atoms_arr[gr] = calloc(atoms_per_group, sizeof(ATOM*));
        if (atoms_arr[gr] == NULL) {
            status = NULL_PTR;
            printf("[ERR %d] Can't allocate group array\n", status);
            exit(status);
        }

        for (int i = 0; i < atoms_per_group; ++i) {
            atoms_arr[gr][i] = init_atom(&status);
        }
    }

    if (status) {
        printf("[ERR %d] Can't allocate atom\n", status);
        exit(status);
    }
    
    // fill group
    for (int gr = 0; gr < groups; ++gr) {
        for (int i = 0; i < atoms_per_group; ++i) {
            float* powers = get_random_powers(groups);
            fill_atom(atoms_arr[gr][i], START_ATOM_RADIUS, COLOR_CODES[gr], powers);
        }
    }

    for (int gr = 0; gr < groups; ++gr) {
        random_position(atoms_arr[gr], atoms_per_group);
    }

    if (DEBUG) {
        printf("[DEBUG] 1st atom\n");
        printf("x:%d y:%d\n", atoms_arr[0][0]->atom->x, atoms_arr[0][0]->atom->y);
        printf("color code:%d\n", atoms_arr[0][0]->color_code);
        printf("powers:%f\n", atoms_arr[0][0]->powers[0]);
        printf("[DEBUG]\n\n");
        //getchar();
    }


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
                    for (int gr = 0; gr < groups; ++gr) {
                        for (int i = 0; i < atoms_count; ++i) {
                            change_radius(atoms_arr[gr][i]->atom, atoms_arr[gr][i]->atom->w + radius_step);
                        }
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS) {
                    for (int gr = 0; gr < groups; ++gr) {
                        for (int i = 0; i < atoms_count; ++i) {
                            change_radius(atoms_arr[gr][i]->atom, atoms_arr[gr][i]->atom->w - radius_step);
                        }
                    }
                }
            }
        }

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        /***********PROCESSING PART***********/
        //need func for update all atoms in arr with their rule (
        for (int gr1 = 0; gr1 < groups; ++gr1) {
            for(int gr2 = 0; gr2 < groups; ++gr2) {
                if (DEBUG) {
                    printf("[DEBUG]\n");
                    printf("gr1: %d\n", gr1);
                    printf("gr2: %d\n", gr2);
                }

                process_groups(atoms_arr, COLOR_CODES[gr1], COLOR_CODES[gr2], atoms_per_group);
            }
        }
        /***********PROCESSING PART***********/


        /***********RENDER PART***********/

        ///////need func for draw all atoms in arr with their color
        for (int gr = 0; gr < groups; ++gr) {
            int* curr_color = COLORS[gr];
            SDL_SetRenderDrawColor(rend, curr_color[R], curr_color[G], curr_color[B], curr_color[A]); // set drawing color for rect
            for (int i = 0; i < atoms_per_group; ++i) {
                SDL_RenderFillRect(rend, atoms_arr[gr][i]->atom); // draw filled rect
            }
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
