#include <SDL2/SDL.h>
#include <stdlib.h>

#include "settings.h"
#include "life.h"

void change_radius(SDL_Rect* rect, int radius) {
    rect->w = radius;
    rect->h = radius;

    if (rect->w <= MIN_ATOM_RADIUS || rect->h <= MIN_ATOM_RADIUS) {
        rect->w = MIN_ATOM_RADIUS;
        rect->h = MIN_ATOM_RADIUS;
    }

    if (rect->w >= MAX_ATOM_RADIUS || rect->h >= MAX_ATOM_RADIUS) {
        rect->w = MAX_ATOM_RADIUS;
        rect->h = MAX_ATOM_RADIUS;
    }

    if (DEBUG) {
        printf("[DEBUG] New radius = '%d'\n", rect->w);
    }
}

ATOM* init_atom(int* status) {
    SDL_Rect* atom_rect = calloc(1, sizeof(SDL_Rect));

    if (atom_rect == NULL) {
        *status = NULL_PTR;
    }

    ATOM* obj = calloc(1, sizeof(ATOM));
    if (obj != NULL) {
        obj->atom = atom_rect;
        obj->color_code = 0;
        obj->powers = NULL;

    } else {
        *status = NULL_PTR;
    }

    return obj;
}

void fill_atom(ATOM* obj, int radius, int color, float* powers) {
    if (obj != NULL) {
        if (obj->atom != NULL) {
            obj->atom->w = radius;
            obj->atom->h = radius;
        }
        obj->color_code = color;
        obj->powers = powers;
    }
}

void destroy_atom(ATOM* obj) {
    if (obj != NULL) {
        if (obj->atom != NULL) {
            free(obj->atom);
        }

        if (obj->powers != NULL) {
            free(obj->powers);
        }
        free(obj);
    }
}

void random_position(ATOM** group, int group_size) {
    for (int i = 0; i < group_size; ++i) {
        group[i]->atom->x = rand() % WIN_W;
        group[i]->atom->y = rand() % WIN_H;
    }
}

float frand() {
    float a = POWER_MAX;
    float res = ((float)rand()/(float)(RAND_MAX)) * a;
    if ((rand() % 2) == 2) {
        res = -res;
    }
    return res;
}

float* get_random_powers(int groups) {
    float* pwrs = calloc(groups, sizeof(int));
    if (pwrs != NULL) {
        for (int i = 0; i < groups; ++i) {
            pwrs[i] = frand();
        }
    }

    return pwrs;
}

void process_groups(ATOM*** all, int color1, int color2, int group_size) {
    ATOM** group1 = all[color1];
    ATOM** group2 = all[color2];
    for (int first = 0; first < group_size; ++first) {
        for (int second = 0; second < group_size; ++second) {
            float fx = 0;
            float fy = 0;

            float dx = group1[first]->atom->x - group2[second]->atom->x;
            float dy = group1[first]->atom->y - group2[second]->atom->y;
            int d = sqrt((dx * dx) + (dy * dy));
            if (d > 0) {
                float F = group1[first]->powers[color2] * 1 / d;
                fx += F * dx;
                fy += F * dy;
                if (DEBUG) {
                    printf("F: %f\n", F);
                }
            }

            group1[first]->atom->x += round(fx);
            group1[first]->atom->y += round(fy);

            if (DEBUG) {
                printf("[DEBUG]\n");
                printf("f: %d, s:%d\n", first, second);
                printf("fx: %f  fy: %f\n", fx, fy);
                printf("dx: %f  dy: %f\n", dx, dy);
                printf("d: %d\n", d);
                printf("i: %d, atom[f]->x: %d  atom[f]->y: %d\n", first, group1[first]->atom->x, group1[first]->atom->y);
                printf("\n");
            }

        }
    }
}
