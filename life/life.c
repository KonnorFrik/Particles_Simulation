#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <math.h>

#include "settings.h"
#include "life.h"
//#include "color.h"

/*
   get filename from user
   get groups count from file
   fill colors 
   if groups more then prepareded colors - generate new color
   fill atoms arr with info from file
 */

int main(int argc, char *argv[]) {
    // for randomise random
    time_t t;
    srand((unsigned) time(&t));

    int status = 0;

    int Win_width = WIN_W;
    int Win_height = WIN_H;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[ERR] Initializing SDL: %s\n", SDL_GetError());
        status = INIT_ERR;
    }

    SDL_Window* window = SDL_CreateWindow("Particles", // creates a window
                                       WINDOW_ZERO_POS,
                                       WINDOW_ZERO_POS,
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

    printf("Enter filename with rule. Empty for random\n>> ");
    char* filename = get_str(&status);

    int groups = 0;

    int COLOR_CODES[];// = { RED, GREEN, BLUE };
    int color_codes_curr_len = 0;

    int* COLORS[ARR_SIZE];
    int colors_curr_len = 0;

    if (filename[0] != '\0') {
        printf("READ RULE\n");

    } else {
        printf("RANDOM\n");
        groups = rand() % MAX_GROUPS;

        fill_colors(COLORS, colors_curr_len, groups);
        // TODO 
    }

    /*****COLOR MODULE*********/
    //int RED_COLOR[4] = { 255, 0, 0, 255 };
    //int GREEN_COLOR[4] = { 0, 255, 0, 255 };
    //int BLUE_COLOR[4] = { 0, 0, 255, 255 };
    //int YELLOW_COLOR[4] = { 0, 255, 255, 255 };
    //int WHITE_COLOR[4] = { 255, 255, 255, 255 };
    //COLORS[RED] = RED_COLOR;
    //COLORS[GREEN] = GREEN_COLOR;
    //COLORS[BLUE] = BLUE_COLOR;
    /*****COLOR MODULE*********/

    int radius_step = 1;

    int atoms_per_group = START_ATOMS_COUNT;
    //int atoms_count = groups * atoms_per_group;

    /************INIT ALL GROUPS ARRAY**************/
    ATOM*** atoms_arr = calloc(groups, sizeof(ATOM**));
    if (atoms_arr == NULL) {
        status = NULL_PTR;
        printf("[ERR %d] Can't allocate atom array\n", status);
        exit(status);
    }
    /************INIT ALL GROUPS ARRAY**************/

    /************INIT ALL ATOM'S IN ALL GROUPS****************/
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
    /************INIT ALL ATOM'S IN ALL GROUPS****************/
    
    /************FILL EACH ATOM BASIC INFO*****************/
    for (int gr = 0; gr < groups; ++gr) {
        float* powers = get_random_powers(groups);

        if (powers == NULL) {
            status = NULL_PTR;
            printf("[ERR %d] Can't allocate powers\n", status);
            exit(status);
        }

        for (int i = 0; i < atoms_per_group; ++i) {
            fill_atom(atoms_arr[gr][i], START_ATOM_RADIUS, COLOR_CODES[gr], powers);
        }
    }

    //for (int i = 0; i < atoms_per_group; ++i) {
        //atoms_arr[RED][i]->powers[RED]     = 1;
        //atoms_arr[RED][i]->powers[GREEN]   = 1;
        //atoms_arr[GREEN][i]->powers[RED]   = 1;
        //atoms_arr[GREEN][i]->powers[GREEN] = 1;
    //}


    //***********SET RANDOM POSITION****************//
    for (int gr = 0; gr < groups; ++gr) {
        random_position(atoms_arr[gr], atoms_per_group);
    }
    /************FILL EACH ATOM BASIC INFO*****************/

    if (DEBUG) {
        printf("[DEBUG] 1st atom\n");
        printf("x:%d y:%d\n", atoms_arr[0][0]->atom->x, atoms_arr[0][0]->atom->y);
        printf("color code:%d\n", atoms_arr[0][0]->color_code);
        printf("powers: ");
        for (int gr = 0; gr < groups; ++gr) {
            printf("%f ", atoms_arr[0][0]->powers[gr]);
        }
        printf("\n");
        printf("[DEBUG]\n\n");
        //getchar();
    }


    SDL_Event event;
    int work = 1;
    int fps = FPS;
    while (work == 1 && status == 0) {
        SDL_Delay(1000 / fps);
        SDL_RenderClear(rend); // clear all screen

        /*******************EVENT PROCESSING*******************************/
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
                        for (int i = 0; i < atoms_per_group; ++i) {
                            change_radius(atoms_arr[gr][i]->atom, atoms_arr[gr][i]->atom->w + radius_step);
                        }
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS) {
                    for (int gr = 0; gr < groups; ++gr) {
                        for (int i = 0; i < atoms_per_group; ++i) {
                            change_radius(atoms_arr[gr][i]->atom, atoms_arr[gr][i]->atom->w - radius_step);
                        }
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    if (DEBUG) { printf("\nNEW RULES\n"); }

                    for (int gr = 0; gr < groups; ++gr) {
                        float* powers = get_random_powers(groups);
                        for (int i = 0; i < atoms_per_group; ++i) {
                            fill_atom(atoms_arr[gr][i], atoms_arr[gr][i]->atom->w, COLOR_CODES[gr], powers);
                        }
                    }
                    if (DEBUG) {
                        printf("powers: ");
                        for (int gr = 0; gr < groups; ++gr) {
                            printf("%f ", atoms_arr[0][0]->powers[gr]);
                        }
                        printf("\n");
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_R) {
                    if (DEBUG) { printf("\nRESET POS\n"); }

                    for (int gr = 0; gr < groups; ++gr) {
                        random_position(atoms_arr[gr], atoms_per_group);
                        for (int elem = 0; elem < atoms_per_group; ++elem) {
                            atoms_arr[gr][elem]->vx = 0;
                            atoms_arr[gr][elem]->vy = 0;
                        }
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    fps++;
                    if (fps >= MAX_FPS) { fps = MAX_FPS; }
                    if (DEBUG) { printf("NEW FPS: %d\n", fps); }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    fps--;
                    if (fps <= MIN_FPS) { fps = MIN_FPS; }
                    if (DEBUG) { printf("NEW FPS: %d\n", fps); }
                }
            }
        }
        /*******************EVENT PROCESSING*******************************/

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        /***********ATOM PROCESSING PART***********/
        for (int gr1 = 0; gr1 < groups; ++gr1) {
            for(int gr2 = 0; gr2 < groups; ++gr2) {
                process_groups(atoms_arr, COLOR_CODES[gr1], COLOR_CODES[gr2], atoms_per_group);
            }
        }

        keep_in_screen(atoms_arr, groups, atoms_per_group, Win_width, Win_height);
        /***********ATOM PROCESSING PART***********/


        /***************RENDER****************/

        ///////need func for draw all atoms in arr with their color
        for (int gr = 0; gr < groups; ++gr) {
            int* curr_color = COLORS[gr];
            SDL_SetRenderDrawColor(rend, curr_color[R], curr_color[G], curr_color[B], curr_color[A]); // set drawing color for rect
            for (int i = 0; i < atoms_per_group; ++i) {
                SDL_RenderFillRect(rend, atoms_arr[gr][i]->atom); // draw filled rect
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); // set color for screen
        SDL_RenderPresent(rend); // apply previous renderer call's
        /***************RENDER****************/
    }

    for (int gr = 0; gr < groups; ++gr) {
        for (int i = 0; i < atoms_per_group; ++i) {
            //free(atoms_arr[gr][i]);
            destroy_atom(atoms_arr[gr][i]);
        }
        free(atoms_arr[gr]);
    }
    free(atoms_arr);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return status;
}
