#include "life.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <time.h>

#include "../io.h"
#include "color.h"
#include "settings.h"

/*
   get filename from user
   get groups count from file
   fill colors
   if groups more then prepareded colors - generate new color
   fill atoms arr with info from file
 */

int read_groups(char* filename, int* status);
float* read_powers_by_group(char* filename, int group_code);
int* fill_color_codes(char* filename, int* status);
int** fill_colors(int groups, int* col_codes, int* status);

int main(int argc, char* argv[]) {
    // for randomise random
    time_t t;
    srand((unsigned)time(&t));

    int status = 0;

    int Win_width = WIN_W;
    int Win_height = WIN_H;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[ERR] Initializing SDL: %s\n", SDL_GetError());
        status = INIT_ERR;
    }

    SDL_Window* window = SDL_CreateWindow("Particles",  // creates a window
                                          WINDOW_ZERO_POS, WINDOW_ZERO_POS, Win_width, Win_height,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, renderer_flags);

    if (window == NULL || rend == NULL) {
        status = INIT_ERR;
        printf("[ERR %d] Initializing window or renderer: %s\n", status, SDL_GetError());
    }

    printf("Enter filename with rule. Empty for random\n>> ");
    char* filename = get_str(&status);

    if (filename == NULL) {
        status = NULL_PTR;
    }

    int* COLOR_CODES = NULL;
    int** COLORS = NULL;

    int is_read = 0;
    if (!status && filename[0] != '\0') {
        if (DEBUG) {
            printf("READ RULE\n");
        }
        is_read = 1;
    }

    int groups = 0;
    if (is_read && !status) {
        // read groups
        groups = read_groups(filename, &status);
        if (groups != 0) {
            COLOR_CODES = fill_color_codes(filename, &status);

        } else {
            status = READ_ERR;
        }

        if (COLOR_CODES != NULL) {
            COLORS = fill_colors(groups, COLOR_CODES, &status);

        } else {
            status = NULL_PTR;
        }

        if (COLORS == NULL) {
            status = NULL_PTR;
        }

    } else {
        // random groups
        groups = rand() % MAX_GROUPS;
        if (groups == 0) {
            groups++;
        }
        COLOR_CODES = calloc(groups, sizeof(int));
        COLORS = calloc(groups, sizeof(int*));

        if (COLOR_CODES != NULL) {
            for (int gr = 0; gr < groups; ++gr) {
                COLOR_CODES[gr] = gr;
            }

        } else {
            status = NULL_PTR;
        }

        if (COLORS != NULL) {
            for (int gr = 0; gr < groups; ++gr) {
                COLORS[gr] = get_color_by_id(gr);
            }

        } else {
            status = NULL_PTR;
        }
    }

    int radius_step = 1;

    int atoms_per_group = START_ATOMS_COUNT;

    // CAN BE ERR HERE - groups may not be readed from file - Clear screen in simul
    /************INIT ALL GROUPS ARRAY**************/
    ATOM*** atoms_arr = NULL;
    if (groups != 0) {
        atoms_arr = calloc(groups, sizeof(ATOM**));
    }
    if (atoms_arr == NULL) {
        status = NULL_PTR;
        printf("[ERR %d] Can't allocate atom array\n", status);
    }
    /************INIT ALL GROUPS ARRAY**************/

    /************INIT ALL ATOM'S IN ALL GROUPS****************/
    for (int gr = 0; gr < groups; ++gr) {  // if groups = 0 - cycle can't run
        atoms_arr[gr] = calloc(atoms_per_group, sizeof(ATOM*));
        if (atoms_arr[gr] == NULL) {
            status = NULL_PTR;
            printf("[ERR %d] Can't allocate group array\n", status);
        }

        for (int i = 0; i < atoms_per_group; ++i) {
            atoms_arr[gr][i] = init_atom(&status);
        }
    }
    if (status) {
        printf("[ERR %d] Can't allocate atom\n", status);
    }
    /************INIT ALL ATOM'S IN ALL GROUPS****************/

    /************FILL EACH ATOM BASIC INFO*****************/
    for (int gr = 0; gr < groups; ++gr) {
        float* powers = NULL;

        if (is_read) {
            // read_powers
            powers = read_powers_by_group(filename, gr);

        } else {
            powers = get_random_powers(groups);
        }

        if (powers == NULL) {
            status = NULL_PTR;
            printf("[ERR %d] Can't allocate powers\n", status);
        }

        for (int i = 0; i < atoms_per_group; ++i) {
            fill_atom(atoms_arr[gr][i], START_ATOM_RADIUS, COLOR_CODES[gr], powers);
        }
    }

    //***********SET RANDOM POSITION****************//
    for (int gr = 0; gr < groups; ++gr) {
        random_position(atoms_arr[gr], atoms_per_group, Win_width, Win_height);
    }
    /************FILL EACH ATOM BASIC INFO*****************/

    if (DEBUG && atoms_arr != NULL) {
        printf("[DEBUG] 1st atom\n");
        printf("x:%d y:%d\n", atoms_arr[0][0]->atom->x, atoms_arr[0][0]->atom->y);
        printf("color code:%d\n", atoms_arr[0][0]->color_code);
        printf("powers: ");
        for (int gr = 0; gr < groups; ++gr) {
            printf("%f ", atoms_arr[0][0]->powers[gr]);
        }
        printf("\n");
        printf("[DEBUG]\n\n");
        // getchar();
    }

    SDL_Event event;
    int work = 1;
    int fps = FPS;
    while (work == 1 && status == 0) {
        SDL_Delay(1000 / fps);
        SDL_RenderClear(rend);  // clear all screen

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
                    if (DEBUG) {
                        printf("\nNEW RULES\n");
                    }

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
                    if (DEBUG) {
                        printf("\nRESET POS\n");
                    }

                    for (int gr = 0; gr < groups; ++gr) {
                        random_position(atoms_arr[gr], atoms_per_group, Win_width, Win_height);
                        for (int elem = 0; elem < atoms_per_group; ++elem) {
                            atoms_arr[gr][elem]->vx = 0;
                            atoms_arr[gr][elem]->vy = 0;
                        }
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    fps++;
                    if (fps >= MAX_FPS) {
                        fps = MAX_FPS;
                    }
                    if (DEBUG) {
                        printf("NEW FPS: %d\n", fps);
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    fps--;
                    if (fps <= MIN_FPS) {
                        fps = MIN_FPS;
                    }
                    if (DEBUG) {
                        printf("NEW FPS: %d\n", fps);
                    }
                }
            }
        }
        /*******************EVENT PROCESSING*******************************/

        SDL_GetWindowSize(window, &Win_width, &Win_height);

        /***********ATOM PROCESSING PART***********/
        for (int gr1 = 0; gr1 < groups; ++gr1) {
            for (int gr2 = 0; gr2 < groups; ++gr2) {
                process_groups(atoms_arr, COLOR_CODES[gr1], COLOR_CODES[gr2], atoms_per_group);
            }
        }

        keep_in_screen(atoms_arr, groups, atoms_per_group, Win_width, Win_height);
        /***********ATOM PROCESSING PART***********/

        /***************RENDER****************/
        for (int gr = 0; gr < groups; ++gr) {
            int* curr_color = COLORS[gr];
            SDL_SetRenderDrawColor(rend, curr_color[R], curr_color[G], curr_color[B],
                                   curr_color[A]);  // set drawing color for rect
            for (int i = 0; i < atoms_per_group; ++i) {
                SDL_RenderFillRect(rend, atoms_arr[gr][i]->atom);  // draw filled rect
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  // set color for screen
        SDL_RenderPresent(rend);                     // apply previous renderer call's
        /***************RENDER****************/
    }

    if (atoms_arr != NULL) {
        for (int gr = 0; gr < groups; ++gr) {
            for (int i = 0; i < atoms_per_group; ++i) {
                destroy_atom(atoms_arr[gr][i]);
            }

            free(atoms_arr[gr]);
        }

        free(atoms_arr);
    }

    if (filename != NULL) {
        free(filename);
    }

    if (COLOR_CODES != NULL) {
        free(COLOR_CODES);
    }

    if (COLORS != NULL) {
        for (int i = MAX_GROUPS; i < groups && groups > MAX_GROUPS; ++i) {
            if (COLORS[i] != NULL) {
                free(COLORS[i]);  // double free err
            }
        }
        free(COLORS);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return status;
}

float* read_powers_by_group(char* filename, int group_code) {
    FILE* file = fopen(filename, "rb");
    float* power = NULL;
    int groups_count = 0;

    if (file != NULL) {
        fread(&groups_count, sizeof(groups_count), 1, file);
        power = calloc(groups_count, sizeof(float));
    }

    if (power != NULL && file != NULL) {
        int group_id = 0;
        int is_find = 0;

        while (!feof(file) && !is_find) {
            fread(&group_id, sizeof(group_id), 1, file);

            if (group_id == group_code) {
                fread(power, sizeof(power[0]), groups_count, file);
                is_find = 1;
            }
        }

        fclose(file);
    }
    return power;
}

int read_groups(char* filename, int* status) {
    FILE* file = fopen(filename, "rb");
    int res = -1;

    if (file != NULL) {
        fread(&res, sizeof(res), 1, file);
        fclose(file);

    } else {
        if (DEBUG) {
            printf("[DEBUG] can't open file in 'read_groups'\n");
        }
        *status = NULL_PTR;
    }

    if (res == -1) {
        *status = READ_ERR;
    }

    return res;
}

int* fill_color_codes(char* filename, int* status) {
    FILE* file = fopen(filename, "rb");
    int groups_count = 0;
    int* res = NULL;

    if (file != NULL) {
        fread(&groups_count, sizeof(groups_count), 1, file);

        if (groups_count == 0) {
            *status = READ_ERR;
        } else {
            res = calloc(groups_count, sizeof(int));
        }

    } else {
        *status = NULL_PTR;
    }

    if (file != NULL && res != NULL) {
        for (int gr = 0; gr < groups_count; ++gr) {
            fread(&(res[gr]), sizeof(res[0]), 1, file);
            fseek(file, sizeof(float) * groups_count, SEEK_CUR);
        }
    }

    return res;
}

int** fill_colors(int groups, int* col_codes, int* status) {
    int** res = calloc(groups, sizeof(int*));

    if (res != NULL) {
        for (int gr = 0; gr < groups; ++gr) {
            res[gr] = get_color_by_id(col_codes[gr]);
        }

    } else {
        *status = NULL_PTR;
    }
    return res;
}
