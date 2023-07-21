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

float* get_powers_by_code(int color) {
    float* pwrs = calloc(1, sizeof(int));
    if (pwrs != NULL) {
        pwrs[color] = -1.4;
    }

    return pwrs;
}

void process_groups(ATOM*** all, int color1, int color2) {
}

