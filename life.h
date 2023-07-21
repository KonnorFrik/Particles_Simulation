#ifndef __LIFE_H__
#define __LIFE_H__

#include <SDL2/SDL.h>
#define MIN_ATOM_RADIUS 1
#define START_ATOM_RADIUS 5
#define MAX_ATOM_RADIUS 100

#define START_ATOMS_COUNT 20

enum color_codes {
    RED = 0,
};

typedef struct {
    SDL_Rect* atom; // x y w h 
    int color_code;
    float* powers;
} ATOM;

void change_radius(SDL_Rect* rect, int radius);
ATOM* init_atom(int* status);
void fill_atom(ATOM* obj, int radius, int color, float* powers);
void destroy_atom(ATOM* obj);
void random_position(ATOM** group, int group_size);
float* get_powers_by_code(int color);

#endif
