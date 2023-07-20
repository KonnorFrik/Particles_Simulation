#ifndef __LIFE_H__
#define __LIFE_H__

#include <SDL2/SDL.h>
#define MIN_ATOM_RADUIS 1
#define MAX_ATOM_RADUIS 100

typedef struct {
    SDL_Rect* atom; // x y w h 
    int color_code;
    int* powers;
} ATOM;

void change_radius(SDL_Rect* rect, int radius);

#endif
