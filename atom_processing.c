#include <SDL2/SDL.h>

#include "settings.h"
#include "life.h"

void change_radius(SDL_Rect* rect, int radius) {
    rect->w = radius;
    rect->h = radius;

    if (rect->w <= MIN_ATOM_RADUIS || rect->h <= MIN_ATOM_RADUIS) {
        rect->w = MIN_ATOM_RADUIS;
        rect->h = MIN_ATOM_RADUIS;
    }

    if (rect->w >= MAX_ATOM_RADUIS || rect->h >= MAX_ATOM_RADUIS) {
        rect->w = MAX_ATOM_RADUIS;
        rect->h = MAX_ATOM_RADUIS;
    }

    if (DEBUG) {
        printf("[DEBUG] New radius = '%d'\n", rect->w);
    }
}
